#include <iostream>
#include <string>
#include "InstalationManager.hpp"
#include "AppBackend.hpp"
#include <curl/curl.h>
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <QStandardPaths>
#include "archive.h"
#include "IRunnable.hpp"
#include "IConfig.hpp"
#include "IGameManager.hpp"
#include "AppInfoParser.hpp"
#include "FileListParser.hpp"
#include "Cleanup.hpp"
#include "Downloader.hpp"
#include "ArchieveInstaller.hpp"
#include "GameParser.hpp"
#include "UpdateInfo.hpp"

namespace im {
	namespace {
		void error(const std::string& what) { throw std::exception(what.c_str()); }

		void error(const QString& what) { throw std::exception(what.toStdString().c_str()); }

		void runAndCheck(IRunnable& runnabe) {
			runnabe.reset();
			if (!runnabe.run()) {
				error(runnabe.getErrorStr());
			}
		}

		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
#ifdef  _WIN32


		HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszDir, LPCSTR lpszPathLink, LPCSTR lpszDesc) {
			HRESULT hres;
			IShellLink* psl;

			// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
			// has already been called.
			hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
			if (SUCCEEDED(hres)) {
				IPersistFile* ppf;

				// Set the path to the shortcut target and add the description. 
				psl->SetPath(lpszPathObj);
				psl->SetWorkingDirectory(lpszDir);
				psl->SetDescription(lpszDesc);

				// Query IShellLink for the IPersistFile interface, used for saving the 
				// shortcut in persistent storage. 
				hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

				if (SUCCEEDED(hres)) {
					WCHAR wsz[MAX_PATH];

					// Ensure that the string is Unicode. 
					MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

					// Add code here to check return value from MultiByteWideChar 
					// for success.

					// Save the link by calling IPersistFile::Save. 
					hres = ppf->Save(wsz, TRUE);
					ppf->Release();
				}
				psl->Release();
			}
			return hres;
		}
#endif
	}

	InstalationManager::InstalationManager() {
		updateInfo_.reset(new UpdateInfo);
		downloader_.reset(new Downloader);
		installer_.reset(new ArchieveInstaller);
		cleanUpper_.reset(new Cleanup);
		appInfoParser_.reset(new AppInfoParser);
		fileListParser_.reset(new FileListParser);
		gameParser_.reset(new GameParser);
	}

	InstalationManager::~InstalationManager() {}

	void InstalationManager::init() {
		updateInfo_->init(*this);
		downloader_->init(*this);
		installer_->init(*this);
		cleanUpper_->init(*this);
		appInfoParser_->init(*this);
		fileListParser_->init(*this);
		gameParser_->init(*this);

		auto& downloadDir = bc::Backend::getBackend().getConfig().getDownloadDir();
		if (!std::filesystem::exists(downloadDir)) {
			try {
				std::filesystem::create_directory(downloadDir);
			} catch (...) {
				//todo
			}
		}
	}

	void InstalationManager::run() {
		try {
			switch (updateInfo_->getUpdateMode()) {
			case UpdateInfo::UpdateMode::LAUNCHBOX:
				updateMainApp();
			default:
				break;
			case UpdateInfo::UpdateMode::GAME:
				updateGame();
				break;
			}
		} catch (std::exception& e) {
			errorEmit(e.what());
			setStateLb(lb::State::ERRORD);
			setVisibleStateLb(lb::VisibleState::HIDDEN);
		} catch (...) {
			errorEmit("Unexpected error ocured while processing");
		}
		cleanUp();
	}

	bool InstalationManager::updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl, bool fullInstall) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::LAUNCHBOX);
		updateInfo_->setActualVersion(version);
		updateInfo_->setFullInstall(fullInstall);
		updateInfo_->setFiles({ {appInfoUrl, "AppInfo.json" }, {gamesRepoUrl, "Games.json"} });
		start();
		return true;
	}

	bool InstalationManager::updateGame(cf::Game& game) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::GAME);
		updateInfo_->setActualGame(game);
		updateInfo_->setActualVersion(game.version);
		updateInfo_->setFullInstall(!game.installed);
		updateInfo_->setFiles({ {game.appInfoUrl.toStdString(), "AppInfo.json" } });
		progress_ = 100;
		setStateLb(lb::State::CHECKING);
		setVisibleStateLb(lb::VisibleState::SHOWED);
		start();
		return true;
	}

	void InstalationManager::updateMainApp() {
		// download appInfo.json and Games.json
		runAndCheck(*downloader_);
		// check if appInfo.json contains newer version = need update
		runAndCheck(*appInfoParser_);
		if (appInfoParser_->needUpdate()) {
			updateApp();
			updateGamePages(); // update Game pages metadata
		}
	}

	void InstalationManager::updateGame() {
		// download game appInfo.json
		runAndCheck(*downloader_);
		// check if appInfo.json contains newer version = need update
		runAndCheck(*appInfoParser_);
		if (appInfoParser_->needUpdate()) {
			updateApp();
			updateGameInfo(); // update Game info 
		}
		updateInfo_->getActualGame().updateChecked = true;
	}

	void InstalationManager::updateApp() {
		setTotal(0);
		updateInfo_->setUpdateVersion(appInfoParser_->getVersionToUpdate());
		updateInfo_->setFiles(appInfoParser_->getFiles());
		progress_ = 100;
		setStateLb(lb::State::CHECKING);
		setVisibleStateLb(lb::VisibleState::SHOWED);
		// download Filelist.json and Patch.json Files
		runAndCheck(*downloader_);
		// Process fileList.json and patch files to get update packets (zip files)
		runAndCheck(*fileListParser_);

		updateStatus(upd::State::DOWNLOADING);
		downloadUpdate();
	}

	void InstalationManager::downloadUpdate() {
		// setUp updateInfo
		updateInfo_->setFiles(fileListParser_->getNeededFiles());
		setTotal(fileListParser_->getBytesToDownload());

		setStateLb(lb::State::DOWNLOADING);
		setVisibleStateLb(lb::VisibleState::SHOWED);
		// download zip packets
		runAndCheck(*downloader_);

		installUpdate();
	}

	void InstalationManager::installUpdate() {
		// set InstallDirectory
		std::filesystem::path destination = updateInfo_->isGameUppdating() ? updateInfo_->getActualGame().gameDir.toStdString() : std::string("../");
		updateInfo_->setInstallDir(destination);

		setStateLb(lb::State::INSTALLING);
		updateStatus(upd::State::INSTALLING);

		// install downloaded packets
		runAndCheck(*installer_);
	}

	void InstalationManager::updateGameInfo() {
		auto& actualGame = updateInfo_->getActualGame();
		actualGame.installed = true;
		actualGame.updateChecked = true;
		actualGame.version = updateInfo_->getUpdateVersion();
		if (actualGame.shortcut) {
			QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
			std::filesystem::path link = desktopPath.toUtf8().constData();
			link /= actualGame.name.toUtf8().constData();
			link += ".lnk";
			std::filesystem::path path = actualGame.gameDir.toUtf8().constData();
			path /= actualGame.execPath.toUtf8().constData();
			std::string ff(path.generic_string().c_str());
			auto res = CreateLink(path.generic_string().c_str(), path.parent_path().generic_string().c_str(), link.generic_string().c_str(), "Sylio shortcut");
			actualGame.shortcutPath = link.generic_string().c_str();
		}
		bc::Backend::getBackend().getGameManager().unLock();
	}

	void InstalationManager::updateGamePages() {
		runAndCheck(*gameParser_);
		if (gameParser_->needUpdate()) {
			// setUp updateInfo
			updateInfo_->setFiles(gameParser_->getFiles());
			setTotal(0);
			// download zip packets containing game pages files
			runAndCheck(*downloader_);
			updateInfo_->setInstallDir(""); // when empty installer uses destination from file vector
			runAndCheck(*installer_);
			gameParser_->updateGamesInfo();
		}
	}

	void InstalationManager::cleanUp() {
		runAndCheck(*cleanUpper_);
		setStateLb(lb::State::COMPLEET);
		setVisibleStateLb(lb::VisibleState::HIDDEN);
		updateEnded(updateInfo_->getUpdateVersion());
		bc::Backend::getBackend().getGameManager().unLock();
	}

	void InstalationManager::setTotal(qint64 tot) {
		totalBytes_ = tot;
		total_ = getMB(totalBytes_);
		setTotalLb(total_);
	}

	void InstalationManager::setProgress() {
		ProgressBytes_ = downloadedBytes_ + unpackedBytes_;
		if (totalBytes_) {
			progress_ = (double(ProgressBytes_) / (2 * totalBytes_)) * 99;
			//std::cout << "progress: " << progress_ << "\n";
		}
	}

	void InstalationManager::downloadStatus(qint64 progress, qint64 total, double speed) {
		downloadedBytes_ += progress;
		if (total) {
			setProgress();
			speed_ = speed; // B/s
			sendDataToBar();
		}
	}

	void InstalationManager::sendDataToBar() {
		updateProgress(progress_);
		setActualLb(getMB(downloadedBytes_));
		setSpeedLb(speed_);
	}

	void InstalationManager::installStatus(qint64 progress) {
		unpackedBytes_ = progress;
		setProgress();
		sendDataToBar();
	}

	void InstalationManager::pause() {
		//if (stage_ == Stage::DOWNLOAD) {
		//	downloader_->pause.clear();
		//	state_ = State::PAUSE;
		//	LoadingBar_->setState(lb::LoadingBar::State::PAUSE);
		//}
	}

	void InstalationManager::resume() {
		//if (stage_ == Stage::DOWNLOAD) {
		//	downloader_->resume.clear();
		//	LoadingBar_->setState(lb::LoadingBar::State::DOWNLOADING);
		//}
	}

	void InstalationManager::stop() {
		//downloader_->stop.clear();
	}

	void InstalationManager::reset() {
		updateInfo_->reset();
		downloader_->reset();
		installer_->reset();
		cleanUpper_->reset();
		appInfoParser_->reset();
		fileListParser_->reset();
		gameParser_.reset();

		totalBytes_ = 0; //total Bytes to download unpack all files together
		downloadedBytes_ = 0; //Bytes downloaded
		unpackedBytes_ = 0; // Bytes Unpacked
		ProgressBytes_ = 0; // dwonloaded + unpacked
		progress_ = 0;
		actual_ = 0;
		total_ = 0;
		speed_ = 0;
		error_ = 0;
		QString errorStr_ = "";

	}
}

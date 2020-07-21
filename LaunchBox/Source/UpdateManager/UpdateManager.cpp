#include <iostream>
#include <string>
#include "UpdateManager.hpp"
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
#include "AppInfoParser.hpp"
#include "FileListParser.hpp"
#include "Cleanup.hpp"
#include "Downloader.hpp"
#include "ArchieveInstaller.hpp"
#include "GameParser.hpp"
#include "UpdateInfo.hpp"

namespace im {
	namespace {

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

	UpdateManager::UpdateManager() {
		updateInfo_.reset(new UpdateInfo);
		downloader_.reset(new Downloader);
		installer_.reset(new ArchieveInstaller);
		cleanUpper_.reset(new Cleanup);
		appInfoParser_.reset(new AppInfoParser);
		fileListParser_.reset(new FileListParser);
		gameParser_.reset(new GameParser);
	}

	UpdateManager::~UpdateManager() {}

	void UpdateManager::init() {
		updateInfo_->init(*this);
		downloader_->init(*this);
		installer_->init(*this);
		cleanUpper_->init(*this);
		appInfoParser_->init(*this);
		fileListParser_->init(*this);
		gameParser_->init(*this);

		auto& downloadDir = bc::Component<cf::IConfig>::get().getDownloadDir();
		if (!std::filesystem::exists(downloadDir)) {
			try {
				std::filesystem::create_directory(downloadDir);
			} catch (...) {
				//todo
			}
		}
	}

	void UpdateManager::run() {
		im::IUpdateManager::State finalState = im::IUpdateManager::State::NONE;
		QString what;
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
			finalState = im::IUpdateManager::State::COMPLEET;
		} catch (AbortException& e) {
			finalState = im::IUpdateManager::State::STOPPED;
		} catch (std::exception& e) {
			finalState = im::IUpdateManager::State::ERRORD;
			what = e.what();
		} catch (...) {
			finalState = im::IUpdateManager::State::ERRORD;
			what = "Unexpected error ocured while processing";
		}
		try {
			cleanUp(finalState, what);
		} catch (...) {}// if cleanup fail its fine
	}

	bool UpdateManager::installMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::LAUNCHBOX);
		updateInfo_->setActualVersion(version);
		updateInfo_->setFullInstall(true);
		updateInfo_->setFiles({ {appInfoUrl, "AppInfo.json" }, {gamesRepoUrl, "Games.json"} });
		start();
		return true;
	}

	bool UpdateManager::updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::LAUNCHBOX);
		updateInfo_->setActualVersion(version);
		updateInfo_->setFullInstall(false);
		updateInfo_->setFiles({ {appInfoUrl, "AppInfo.json" }, {gamesRepoUrl, "Games.json"} });
		start();
		return true;
	}

	bool UpdateManager::installGame(cf::Game& game, const QString& gamePath, bool shortcut) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::GAME);
		updateInfo_->setActualGame(game);
		auto& gg = game.appInfoUrl.toStdString();
		auto& actualGame = updateInfo_->getActualGame();
		actualGame.gameDir = gamePath;
		actualGame.shortcut = shortcut;
		updateInfo_->setActualVersion(game.version);
		updateInfo_->setFullInstall(!game.installed);
		updateInfo_->setFiles({ {game.appInfoUrl.toStdString(), "AppInfo.json" } });
		auto& hh = updateInfo_->getFiles().at(0);
		//progress_ = 100;
		//emit emitState(lb::State::CHECKING);
		//emit emitVisibleState(lb::VisibleState::SHOWED);
		start();
		return true;
	}

	bool UpdateManager::updateGame(cf::Game& game) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		updateInfo_->setUpdateMode(UpdateInfo::UpdateMode::GAME);
		updateInfo_->setActualGame(game);
		updateInfo_->setActualVersion(game.version);
		updateInfo_->setFullInstall(!game.installed);

		updateInfo_->setFiles({ {game.appInfoUrl.toStdString(), "AppInfo.json" } });
		//progress_ = 100;
		//emit emitState(lb::State::CHECKING);
		//emit emitVisibleState(lb::VisibleState::SHOWED);
		start();
		return true;
	}

	void UpdateManager::updateMainApp() {
		// download appInfo.json and Games.json
		emitState(im::IUpdateManager::State::CHECKING);
		downloader_->run();
		// check if appInfo.json contains newer version = need update
		appInfoParser_->run();
		if (appInfoParser_->needUpdate()) {
			updateApp();
		}
		gameParser_->run();
		if (gameParser_->needUpdate()) {
			updateGamePages(); // update Game pages metadata
		}
	}

	void UpdateManager::updateGame() {
		progress_ = 100;
		emitState(im::IUpdateManager::State::CHECKING);
		emitVisibleState(im::IUpdateManager::VisibleState::SHOWED);
		sendDataToBar();
		// download game appInfo.json
		auto& hh = updateInfo_->getFiles().at(0);
		downloader_->run();
		// check if appInfo.json contains newer version = need update
		appInfoParser_->run();
		if (appInfoParser_->needUpdate()) {
			updateApp();
			updateGameInfo(); // update Game info 
		}
		auto& actual = updateInfo_->getActualGame();
		actual.updateChecked = true;
	}

	void UpdateManager::updateApp() {
		setTotal(0);
		updateInfo_->setUpdateVersion(appInfoParser_->getVersionToUpdate());
		updateInfo_->setFiles(appInfoParser_->getFiles());
		progress_ = 100;
		emitState(im::IUpdateManager::State::CHECKING);
		emitVisibleState(im::IUpdateManager::VisibleState::SHOWED);
		// download Filelist.json and Patch.json Files
		downloader_->run();
		// Process fileList.json and patch files to get update packets (zip files)
		fileListParser_->run();

		downloadUpdate();
		installUpdate();
	}

	void UpdateManager::downloadUpdate() {
		// setUp updateInfo
		progress_ = 0;
		updateInfo_->setFiles(fileListParser_->getNeededFiles());
		setTotal(fileListParser_->getBytesToDownload());
		sendDataToBar();

		emitState(im::IUpdateManager::State::DOWNLOADING);
		emitVisibleState(im::IUpdateManager::VisibleState::SHOWED);
		// download zip packets
		downloader_->run();
	}

	void UpdateManager::installUpdate() {
		// set InstallDirectory
		std::filesystem::path destination = updateInfo_->isGameUppdating() ? updateInfo_->getActualGame().gameDir.toStdString() : std::string("../");
		updateInfo_->setInstallDir(destination);

		emitState(im::IUpdateManager::State::INSTALLING);
		// install downloaded packets
		installer_->run();
	}

	void UpdateManager::updateGameInfo() {
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
		//bc::Component<gm::IGameManager>::get().unLock();
	}

	void UpdateManager::updateGamePages() {
		emitState(im::IUpdateManager::State::UPDATING_GAME_PAGES);
		// setUp updateInfo
		updateInfo_->setFiles(gameParser_->getFiles());
		setTotal(0);
		// download zip packets containing game pages files
		downloader_->run();
		updateInfo_->setInstallDir(""); // when empty installer uses destination from file vector
		installer_->run();
		gameParser_->updateGamesInfo();
	}

	void UpdateManager::cleanUp(im::IUpdateManager::State finalState, const QString& errorWhat) {
		cleanUpper_->run();
		emitState(finalState);
		if (finalState == im::IUpdateManager::State::ERRORD && !errorWhat.isEmpty())
			errorEmit(errorWhat);
		emitVisibleState(im::IUpdateManager::VisibleState::HIDDEN);
		updateEnded(updateInfo_->getUpdateVersion());
		//bc::Component<gm::IGameManager>::get().unLock();
	}

	void UpdateManager::setTotal(qint64 tot) {
		totalBytes_ = tot;
		total_ = getMB(totalBytes_);
		setTotalLb(total_);
	}

	void UpdateManager::setProgress() {
		ProgressBytes_ = downloadedBytes_ + unpackedBytes_;
		if (totalBytes_) {
			progress_ = (double(ProgressBytes_) / (2 * totalBytes_)) * 99;
			//std::cout << "progress: " << progress_ << "\n";
		}
	}

	void UpdateManager::downloadStatus(qint64 progress, qint64 total, double speed) {
		downloadedBytes_ += progress;
		if (total) {
			setProgress();
			speed_ = speed; // B/s
			sendDataToBar();
		}
	}

	void UpdateManager::sendDataToBar() {
		updateProgress(progress_);
		setActualLb(getMB(downloadedBytes_));
		setSpeedLb(speed_);
	}

	void UpdateManager::installStatus(qint64 progress) {
		unpackedBytes_ = progress;
		setProgress();
		sendDataToBar();
	}

	void UpdateManager::pause() {
		updateInfo_->pause.clear();
	}

	void UpdateManager::resume() {
		updateInfo_->resume.clear();
	}

	void UpdateManager::stop() {
		updateInfo_->stop.clear();
	}

	void UpdateManager::reset() {
		updateInfo_->reset();
		downloader_->reset();
		installer_->reset();
		cleanUpper_->reset();
		appInfoParser_->reset();
		fileListParser_->reset();
		gameParser_->reset();


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

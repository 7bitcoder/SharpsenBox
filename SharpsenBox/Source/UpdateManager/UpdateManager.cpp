#include <iostream>
#include <string>
#include <QStandardPaths>
#include <QDir>
#include <curl/curl.h>
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "archive.h"
#include "IRunnable.hpp"
#include "IConfig.hpp"
#include "AppInfoParser.hpp"
#include "UpdateManager.hpp"
#include "FileListParser.hpp"
#include "Cleanup.hpp"
#include "Downloader.hpp"
#include "ArchieveInstaller.hpp"
#include "GameParser.hpp"
#include "UpdateInfo.hpp"

namespace sb {
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
		_UpdateInfo.reset(new UpdateInfo);
		downloader_.reset(new Downloader);
		installer_.reset(new ArchieveInstaller);
		cleanUpper_.reset(new Cleanup);
		appInfoParser_.reset(new AppInfoParser);
		fileListParser_.reset(new FileListParser);
		gameParser_.reset(new GameParser);
	}

	UpdateManager::~UpdateManager() {}

	void UpdateManager::Init() {
		_UpdateInfo->Init(*this);
		downloader_->Init(*this);
		installer_->Init(*this);
		cleanUpper_->Init(*this);
		appInfoParser_->Init(*this);
		fileListParser_->Init(*this);
		gameParser_->Init(*this);

		auto& downloadDir = Component<IConfig>::Get().GetDownloadDir();
		if (!QDir().exists(downloadDir)) {
			try {
				QDir().mkdir(downloadDir);
			} catch (...) {
				//todo
			}
		}
	}

	void UpdateManager::run() {
		IUpdateManager::State finalState = IUpdateManager::State::NONE;
		QString what;
		try {
			switch (_UpdateInfo->getUpdateMode()) {
			case UpdateInfo::UpdateMode::SHARPSENBOX:
				updateMainApp();
			default:
				break;
			case UpdateInfo::UpdateMode::GAME:
				updateGame();
				break;
			}
			finalState = IUpdateManager::State::COMPLEET;
		} catch (AbortException&) {
			finalState = IUpdateManager::State::STOPPED;
		} catch (std::exception& e) {
			finalState = IUpdateManager::State::ERRORD;
			what = e.what();
		} catch (...) {
			finalState = IUpdateManager::State::ERRORD;
			what = "Unexpected Error ocured while processing";
		}
		try {
			cleanUp(finalState, what);
		} catch (...) {}// if cleanup fail its fine
	}

	bool UpdateManager::installMainApp(QString version, QString appInfoUrl, QString gamesRepoUrl) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		_UpdateInfo->setUpdateMode(UpdateInfo::UpdateMode::SHARPSENBOX);
		_UpdateInfo->setActualVersion(version);
		_UpdateInfo->setFullInstall(true);
		_UpdateInfo->setFiles({ {appInfoUrl, "AppInfo.json" }, {gamesRepoUrl, "Games.json"} });
		start();
		return true;
	}

	bool UpdateManager::updateMainApp(QString version, QString appInfoUrl, QString gamesRepoUrl) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		_UpdateInfo->setUpdateMode(UpdateInfo::UpdateMode::SHARPSENBOX);
		_UpdateInfo->setActualVersion(version);
		_UpdateInfo->setFullInstall(false);
		_UpdateInfo->setFiles({ {appInfoUrl, "AppInfo.json" }, {gamesRepoUrl, "Games.json"} });
		start();
		return true;
	}

	bool UpdateManager::installGame(Game& game, const QString& gamePath, bool shortcut) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		_UpdateInfo->setUpdateMode(UpdateInfo::UpdateMode::GAME);
		_UpdateInfo->setActualGame(game);
		auto gg = game.GameInfoUrl.toStdString();
		auto& actualGame = _UpdateInfo->getActualGame();
		actualGame.GameDir = gamePath;
		actualGame.HasShortcut = shortcut;
		_UpdateInfo->setActualVersion(game.Version);
		_UpdateInfo->setFullInstall(!game.IsInstalled);
		_UpdateInfo->setFiles({ {game.GameInfoUrl, "AppInfo.json" } });
		auto& hh = _UpdateInfo->getFiles().at(0);
		//progress_ = 100;
		//emit emitState(State::CHECKING);
		//emit emitVisibleState(VisibleState::SHOWED);
		start();
		return true;
	}

	bool UpdateManager::updateGame(Game& game) {
		if (isRunning())
			return false;
		reset();
		setTotal(0);
		_UpdateInfo->setUpdateMode(UpdateInfo::UpdateMode::GAME);
		_UpdateInfo->setActualGame(game);
		_UpdateInfo->setActualVersion(game.Version);
		_UpdateInfo->setFullInstall(!game.IsInstalled);

		_UpdateInfo->setFiles({ {game.GameInfoUrl, "AppInfo.json" } });
		//progress_ = 100;
		//emit emitState(State::CHECKING);
		//emit emitVisibleState(VisibleState::SHOWED);
		start();
		return true;
	}

	void UpdateManager::updateMainApp() {
		// download appInfo.json and Games.json
		emitState(IUpdateManager::State::CHECKING);
		downloader_->Run();
		// check if appInfo.json contains newer version = need Update
		appInfoParser_->Run();
		if (appInfoParser_->needUpdate()) {
			updateApp();
		}
		gameParser_->Run();
		if (gameParser_->needUpdate()) {
			updateGamePages(); // Update Game pages metadata
		}
	}

	void UpdateManager::updateGame() {
		progress_ = 100;
		emitState(IUpdateManager::State::CHECKING);
		emitVisibleState(IUpdateManager::VisibleState::SHOWED);
		sendDataToBar();
		// download game appInfo.json
		auto& hh = _UpdateInfo->getFiles().at(0);
		downloader_->Run();
		// check if appInfo.json contains newer version = need Update
		appInfoParser_->Run();
		if (appInfoParser_->needUpdate()) {
			updateApp();
			updateGameInfo(); // Update Game info 
		}
		auto& actual = _UpdateInfo->getActualGame();
		actual.UpdateChecked = true;
	}

	void UpdateManager::updateApp() {
		setTotal(0);
		_UpdateInfo->setUpdateVersion(appInfoParser_->getVersionToUpdate());
		_UpdateInfo->setFiles(appInfoParser_->getFiles());
		progress_ = 100;
		emitState(IUpdateManager::State::CHECKING);
		emitVisibleState(IUpdateManager::VisibleState::SHOWED);
		// download Filelist.json and Patch.json Files
		downloader_->Run();
		// Process fileList.json and patch files to get Update packets (zip files)
		fileListParser_->Run();

		downloadUpdate();
		installUpdate();
	}

	void UpdateManager::downloadUpdate() {
		// setUp UpdateInfo
		progress_ = 0;
		_UpdateInfo->setFiles(fileListParser_->getNeededFiles());
		setTotal(fileListParser_->getBytesToDownload());
		sendDataToBar();

		emitState(IUpdateManager::State::DOWNLOADING);
		emitVisibleState(IUpdateManager::VisibleState::SHOWED);
		// download zip packets
		downloader_->Run();
	}

	void UpdateManager::installUpdate() {
		// set InstallDirectory
		QString destination = _UpdateInfo->isGameUppdating() ? _UpdateInfo->getActualGame().GameDir : "../";
		_UpdateInfo->setInstallDir(destination);

		emitState(IUpdateManager::State::INSTALLING);
		// install downloaded packets
		installer_->Run();
	}

	void UpdateManager::updateGameInfo() {
		auto& actualGame = _UpdateInfo->getActualGame();
		actualGame.IsInstalled = true;
		actualGame.UpdateChecked = true;
		actualGame.Version = _UpdateInfo->getUpdateVersion();
		if (actualGame.HasShortcut) {
			QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
			std::filesystem::path link = desktopPath.toUtf8().constData();
			link /= actualGame.Title.toUtf8().constData();
			link += ".lnk";
			std::filesystem::path path = actualGame.GameDir.toUtf8().constData();
			path /= actualGame.ExecutablePath.toUtf8().constData();
			std::string ff(path.generic_string().c_str());
			auto res = CreateLink(path.generic_string().c_str(), path.parent_path().generic_string().c_str(), link.generic_string().c_str(), "Sylio shortcut");
			actualGame.ShortcutPath = link.generic_string().c_str();
		}
		//Component<IGameManager>::get().unLock();
	}

	void UpdateManager::updateGamePages() {
		emitState(IUpdateManager::State::UPDATING_GAME_PAGES);
		// setUp UpdateInfo
		_UpdateInfo->setFiles(gameParser_->getFiles());
		setTotal(0);
		// download zip packets containing game pages files
		downloader_->Run();
		_UpdateInfo->setInstallDir(""); // when empty installer uses destination from file vector
		installer_->Run();
		gameParser_->updateGamesInfo();
	}

	void UpdateManager::cleanUp(IUpdateManager::State finalState, const QString& errorWhat) {
		cleanUpper_->Run();
		if (finalState == IUpdateManager::State::ERRORD && !errorWhat.isEmpty())
			errorEmit(errorWhat);
		else {
			emitState(finalState);
			updateEnded(_UpdateInfo->getUpdateVersion());
		}
		emitVisibleState(IUpdateManager::VisibleState::HIDDEN);
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
		_UpdateInfo->pause.clear();
	}

	void UpdateManager::resume() {
		_UpdateInfo->resume.clear();
	}

	void UpdateManager::stop() {
		_UpdateInfo->stop.clear();
	}

	void UpdateManager::reset() {
		_UpdateInfo->Reset();
		downloader_->Reset();
		installer_->Reset();
		cleanUpper_->Reset();
		appInfoParser_->Reset();
		fileListParser_->Reset();
		gameParser_->Reset();


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

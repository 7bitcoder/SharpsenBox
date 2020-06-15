#include <iostream>
#include <string>
#include "InstalationManager.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include <curl/curl.h>
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <QStandardPaths>
#include "archive.h"
#include "GameManager.hpp"

namespace bb {
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

	InstalationManager::~InstalationManager() {
		downloader_.exit();
		downloader_.terminate();
		downloader_.wait();
		installer_.exit();
		installer_.terminate();
		installer_.wait();
		cleanUpper_.exit();
		cleanUpper_.terminate();
		cleanUpper_.wait();
	}

	void InstalationManager::updateMainApp(QString version, std::filesystem::path appInfoUrl) {
		reset();
		setTotal(0);
		appInfoParser_.setVerToCheck(version);
		files_ = files{ {appInfoUrl, "AppInfo.json" } };
		downloader_.setFilestoDownload(files_);
		stage_ = Stage::DOWNLOAD;
		LoadingBar_->setState(lb::LoadingBar::State::CHECKING);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
		connect(&downloader_, &Downloader::ended, this, &InstalationManager::appInfoDownloaded);
		connect(&downloader_, &Downloader::error, this, &InstalationManager::errorCatched);
		downloader_.start();
	}

	void InstalationManager::updateGame(cf::Game& game) {
		reset();
		setTotal(0);
		actualGame_ = &game;
		appInfoParser_.setVerToCheck(game.version);
		files_ = files{ {game.appInfoUrl.toStdString(), "AppInfo.json" } };
		downloader_.setFilestoDownload(files_);
		stage_ = Stage::DOWNLOAD;
		progress_ = 100;
		LoadingBar_->setState(lb::LoadingBar::State::CHECKING);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
		connect(&downloader_, &Downloader::ended, this, &InstalationManager::appInfoDownloaded);
		connect(&downloader_, &Downloader::error, this, &InstalationManager::errorCatched); //todo przenies errory do konstruktora
		downloader_.start();
	}

	void InstalationManager::appInfoDownloaded() {
		connect(&appInfoParser_, &AppInfoParser::parseEnded, this, &InstalationManager::appInfoParserEnded);
		disconnect(&downloader_, &Downloader::ended, this, &InstalationManager::appInfoDownloaded);
		appInfoParser_.parse();
	}

	void InstalationManager::appInfoParserEnded() {
		if (appInfoParser_.needUpdate()) {
			install(appInfoParser_.getNeededFiles(), appInfoParser_.getBytesToDownload(), actualGame_);
			updateStatus(true);
		} else {
			updateStatus(false);
			if(actualGame_)
				actualGame_->updateChecked = true;
		}
	}

	void InstalationManager::install(files files, qint64 tot, cf::Game* game) {
		reset();
		setTotal(tot);
		actualGame_ = game;
		files_ = files;
		downloader_.setFilestoDownload(files_);
		installer_.setUnpackFiles(files_);
		installDir_ = actualGame_ ? actualGame_->gameDir.toStdString() : std::string("../");
		stage_ = Stage::DOWNLOAD;
		LoadingBar_->setState(lb::LoadingBar::State::DOWNLOADING);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
		connect(&downloader_, &Downloader::statusSignal, this, &InstalationManager::downloadStatus);
		connect(&downloader_, &Downloader::ended, this, &InstalationManager::downloadEnded);
		connect(&downloader_, &Downloader::error, this, &InstalationManager::errorCatched);
		connect(&installer_, &ArchieveInstaller::statusSignal, this, &InstalationManager::installStatus);
		connect(&installer_, &ArchieveInstaller::ended, this, &InstalationManager::installEnded);
		connect(&installer_, &ArchieveInstaller::error, this, &InstalationManager::errorCatched);
		connect(&cleanUpper_, &cu::Cleanup::ended, this, &InstalationManager::cleanUpEnded);
		connect(&cleanUpper_, &cu::Cleanup::error, this, &InstalationManager::errorCatched);
		downloader_.start();
		installer_.setInstalationDir(installDir_);
	}

	void InstalationManager::downloadEnded(bool cancelled) {
		cancel_ = cancelled;
		if (cancel_) {
			LoadingBar_->setState(lb::LoadingBar::State::STOPPED);
			cleanUpper_.start();
		} else {
			stage_ = Stage::INSTALL;
			LoadingBar_->setState(lb::LoadingBar::State::INSTALLING);
			installer_.start();
		}
	}

	void InstalationManager::installEnded() {
		disconnectAll();
		if (actualGame_) {
			actualGame_->installed = true;
			if (actualGame_->shortcut) {
				QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
				std::filesystem::path link = desktopPath.toUtf8().constData();
				link /= actualGame_->name.toUtf8().constData();
				link += ".lnk";
				std::filesystem::path path = actualGame_->gameDir.toUtf8().constData();
				path /= actualGame_->execPath.toUtf8().constData();
				std::string ff(path.generic_string().c_str());
				auto res = CreateLink(path.generic_string().c_str(), path.parent_path().generic_string().c_str(), link.generic_string().c_str(), "Sylio shortcut");
				actualGame_->shortcutPath = link.generic_string().c_str();
				actualGame_->updateChecked = true;
			}
		}
		gm::GameManager::getObject().unLock();
		cleanUpper_.start();
	}

	void InstalationManager::cleanUpEnded() {
		progress_ = 100;
		sendDataToBar();
		LoadingBar_->reset();
		if (!cancel_)
			LoadingBar_->setState(lb::LoadingBar::State::COMPLEET);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::HIDDEN);
		updateEnded(appInfoParser_.getVer());
	}

	void InstalationManager::setTotal(qint64 tot) {
		totalBytes_ = tot;
		total_ = getMB(totalBytes_);
		LoadingBar_->setTotal(total_);
	}

	void InstalationManager::setProgress() {
		ProgressBytes_ = downloadedBytes_ + unpackedBytes_;
		if (totalBytes_) {
			progress_ = (double(ProgressBytes_) / (2 * totalBytes_)) * 99;
			//std::cout << "progress: " << progress_ << "\n";
		}
	}

	Q_INVOKABLE std::string InstalationManager::getName() {
		return TYPENAME(InstalationManager);
	}

	void InstalationManager::downloadStatus(qint64 progress, qint64 total, double speed) {
		downloadedBytes_ = progress;
		if (total) {
			setProgress();
			speed_ = speed; // B/s
			sendDataToBar();
		}
	}

	void InstalationManager::sendDataToBar() {
		LoadingBar_->setProgress(progress_);
		LoadingBar_->setActual(getMB(downloadedBytes_));
		LoadingBar_->setSpeed(speed_);
	}

	void InstalationManager::installStatus(qint64 progress) {
		unpackedBytes_ = progress;
		setProgress();
		sendDataToBar();
	}

	void InstalationManager::TotalSize(qint64 total) {}

	void InstalationManager::pause() {
		if (stage_ == Stage::DOWNLOAD) {
			downloader_.pause.clear();
			state_ = State::PAUSE;
			LoadingBar_->setState(lb::LoadingBar::State::PAUSE);
		}
	}

	void InstalationManager::resume() {
		if (stage_ == Stage::DOWNLOAD) {
			downloader_.resume.clear();
			LoadingBar_->setState(lb::LoadingBar::State::DOWNLOADING);
		}
	}

	void InstalationManager::stop() {
		downloader_.stop.clear();
	}

	void InstalationManager::init() {
		downloadDir_ = cf::Config::getObject().getDownloadDir();
		if (!std::filesystem::exists(downloadDir_)) {
			try {
				std::filesystem::create_directory(downloadDir_);
			} catch (...) {
				//todo
			}
		}
		LoadingBar_ = &lb::LoadingBar::getObject();
	}

	void InstalationManager::reset() {
		downloader_.reset();
		installer_.reset();
		appInfoParser_.reset();
		totalBytes_ = 0; //total Bytes to download unpack all files together
		downloadedBytes_ = 0; //Bytes downloaded
		unpackedBytes_ = 0; // Bytes Unpacked
		ProgressBytes_ = 0; // dwonloaded + unpacked
		progress_ = 0;
		actual_ = 0;
		total_ = 0;
		speed_ = 0;
		error_ = 0;
		actualGame_ = nullptr;
		onlyDownload = false;
		QString errorStr_ = "";
		files_.clear();
		stage_ = Stage::NONE;
		state_ = State::CHECKING;
		visibleState_ = VisibleState::HIDDEN;
		disconnect(&downloader_, &Downloader::statusSignal, this, &InstalationManager::downloadStatus);
		disconnect(&downloader_, &Downloader::ended, this, &InstalationManager::downloadEnded);
		disconnect(&downloader_, &Downloader::error, this, &InstalationManager::errorCatched);
		disconnect(&installer_, &ArchieveInstaller::statusSignal, this, &InstalationManager::installStatus);
		disconnect(&installer_, &ArchieveInstaller::ended, this, &InstalationManager::installEnded);
		disconnect(&installer_, &ArchieveInstaller::error, this, &InstalationManager::errorCatched);
	}



	void InstalationManager::errorCatched(int code) {
		error_ = code;
		if (stage_ == Stage::DOWNLOAD) {
			switch (code) {
			case CURLE_URL_MALFORMAT:
				errorStr_ = "Wrong file request";
				break;
			case CURLE_REMOTE_ACCESS_DENIED:
			case CURLE_FTP_ACCEPT_FAILED:
			case CURLE_FTP_WEIRD_PASS_REPLY:
			case CURLE_FTP_ACCEPT_TIMEOUT:
			case CURLE_GOT_NOTHING:
				errorStr_ = "Server error ocured";
				break;
			case CURLE_RECV_ERROR:
				errorStr_ = "Receiving data error";
				break;
			case CURLE_COULDNT_RESOLVE_HOST:
				errorStr_ = "Could not reach server";
				break;
			case CURLE_COULDNT_CONNECT:
			case CURLE_FTP_CANT_GET_HOST:
				errorStr_ = "Could not connect to Server";
				break;
			case CURLE_OUT_OF_MEMORY:
				errorStr_ = "Out of memeory, could not allocate memory for downloaded files";
				break;
			case CURLE_WRITE_ERROR:
				errorStr_ = "Could not save files on disk";
				break;
			case CURLE_OPERATION_TIMEDOUT:
				errorStr_ = "Connection timeout";
				break;
			case 0: // exception catched
			default:
				errorStr_ = "Unknown error while doanloading data";
				break;
			}
		} else {
			switch (code) {
			case -1: //while deleting archieves
				errorStr_ = "Error ocured while deleting instalation files";
				break;
			case ARCHIVE_FATAL:
				errorStr_ = "Installing Fatal error, check memory disk space";
				break;
			case ARCHIVE_EOF:
			case ARCHIVE_WARN:
			case ARCHIVE_FAILED:
			default:
				errorStr_ = "Unknown error while installing data";
				break;
			}
		}
		LoadingBar_->reset();
		LoadingBar_->setState(lb::LoadingBar::State::ERRORD);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::HIDDEN);
		LoadingBar_->setError(code, errorStr_);
		gm::GameManager::getObject().unLock();
		errorEmit();
	}
}

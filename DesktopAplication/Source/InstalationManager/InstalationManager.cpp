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

namespace bb {
	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
#ifdef  _WIN32


		HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc) {
			HRESULT hres;
			IShellLink* psl;

			// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
			// has already been called.
			hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
			if (SUCCEEDED(hres)) {
				IPersistFile* ppf;

				// Set the path to the shortcut target and add the description. 
				psl->SetPath(lpszPathObj);
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
		ftp_.exit();
		ftp_.terminate();
		ftp_.wait();
	}

	void InstalationManager::setTotal(qint64 tot) { 
		totalBytes_ = tot;
		total_ = getMB(totalBytes_);
		LoadingBar_->setTotal(total_);
	}

	void InstalationManager::setProgress() {
		ProgressBytes_ = downloadedBytes_ + unpackedBytes_;
		if (totalBytes_) {
			progress_ = (double(ProgressBytes_)/ (2 * totalBytes_)) * 99;
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
			speed_ =  speed; // B/s
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

	void InstalationManager::pauseD() {
		ftp_.pause.clear();
		state_ = State::PAUSE;
		LoadingBar_->setState(lb::LoadingBar::State::PAUSE);
	}

	void InstalationManager::resumeD() {
		ftp_.resume.clear();
		LoadingBar_->setState(stage_ == Stage::DOWNLOAD ? lb::LoadingBar::State::DOWNLOADING : lb::LoadingBar::State::INSTALLING);
	}

	void InstalationManager::stopD() {
		ftp_.stop.clear();
		LoadingBar_->setState( lb::LoadingBar::State::STOPPED );
		LoadingBar_->setVisibleState( lb::LoadingBar::VisibleState::HIDDEN );
	}

	
	void InstalationManager::ftpEnded() {
		stage_ = Stage::INSTALL;
		LoadingBar_->setState(lb::LoadingBar::State::INSTALLING);
		if(!onlyDownload)
			installer_.start();
		downloadEnded();
	}

	void InstalationManager::archieveEnded() {
		installEnded();
		disconnectAll();
		if (actualGame_) {
			actualGame_->installed = true;
			if (actualGame_->shortcut) {
				QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
				std::filesystem::path link = desktopPath.toUtf8().constData();
				link /= actualGame_->name.toUtf8().constData();
				link += ".lnk";
				std::filesystem::path path = actualGame_->gameDir.toUtf8().constData();
				path /= actualGame_->execDir.toUtf8().constData();
				std::string ff(path.generic_string().c_str());
				auto res = CreateLink(path.generic_string().c_str(), link.generic_string().c_str(), "Sylio shortcut");
				actualGame_->shortcutPath = link.generic_string().c_str();
			}
			
		}
		progress_ = 100;
		sendDataToBar();
		LoadingBar_->setState(lb::LoadingBar::State::COMPLEET);
		clearFilesEnded();
	}

	void InstalationManager::downloadFile(std::filesystem::path fileName, qint64 tot) {
		downloadFiles({ fileName }, tot);
	}

	void InstalationManager::installFile(std::filesystem::path fileName, qint64 tot, std::filesystem::path dir, cf::Game* game) {
		installFiles({ fileName }, tot, dir, game);
	}

	void InstalationManager::downloadFiles(files files, qint64 tot) {
		reset();
		setTotal(tot);
		files_ = files;
		onlyDownload = true;
		ftp_.setFilestoDownload(files_);
		download();
	}

	void InstalationManager::installFiles(files files, qint64 tot, std::filesystem::path dir, cf::Game* game) {
		reset();
		setTotal(tot);
		actualGame_ = game;
		files_ = files;
		onlyDownload = false;
		ftp_.setFilestoDownload(files_);
		installer_.setUnpackFiles(files_);
		installDir_ = dir;
		install();
	}
	
	void InstalationManager::download() {
		stage_ = Stage::DOWNLOAD;
		progress_ = 100; //for checking state
		LoadingBar_->setState(lb::LoadingBar::State::CHECKING);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
		connect(&ftp_, &FtpDownloader::statusSignal, this, &InstalationManager::downloadStatus);
		connect(&ftp_, &FtpDownloader::ended, this, &InstalationManager::ftpEnded); // archieve ended is final stage skipping archeve install
		connect(&ftp_, &FtpDownloader::error, this, &InstalationManager::errorCatched);
		ftp_.start();
	}

	void InstalationManager::install() {
		stage_ = Stage::DOWNLOAD;
		LoadingBar_->setState(lb::LoadingBar::State::DOWNLOADING);
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
		connect(&ftp_, &FtpDownloader::statusSignal, this, &InstalationManager::downloadStatus);
		connect(&ftp_, &FtpDownloader::ended, this, &InstalationManager::ftpEnded);
		connect(&ftp_, &FtpDownloader::error, this, &InstalationManager::errorCatched);
		connect(&installer_, &ArchieveInstaller::statusSignal, this, &InstalationManager::installStatus);
		connect(&installer_, &ArchieveInstaller::ended, this, &InstalationManager::archieveEnded);
		connect(&installer_, &ArchieveInstaller::error, this, &InstalationManager::errorCatched);
		ftp_.start();
		installer_.setInstalationDir(installDir_);
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
		disconnect(&ftp_, &FtpDownloader::statusSignal, this, &InstalationManager::downloadStatus);
		disconnect(&ftp_, &FtpDownloader::ended, this, &InstalationManager::ftpEnded);
		disconnect(&ftp_, &FtpDownloader::error, this, &InstalationManager::errorCatched);
		disconnect(&installer_, &ArchieveInstaller::statusSignal, this, &InstalationManager::installStatus);
		disconnect(&installer_, &ArchieveInstaller::ended, this, &InstalationManager::archieveEnded);
		disconnect(&installer_, &ArchieveInstaller::error, this, &InstalationManager::errorCatched);
	}

	void InstalationManager::installGame(cf::Game& game) {
		installFile(game.url.toUtf8().constData(), game.size, game.gameDir.toUtf8().constData(), &game);
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
			default:
				errorStr_ = "Unknown error while installing data";
				break;
			}
		}
		LoadingBar_->setState(lb::LoadingBar::State::ERRORD);
		LoadingBar_->setError(code, errorStr_);
		errorEmit();
	} 

	
}
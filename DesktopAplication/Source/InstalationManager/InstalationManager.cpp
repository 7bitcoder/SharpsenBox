#include <iostream>
#include <string>
#include "InstalationManager.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include <curl/curl.h>

namespace bb {
	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
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
		try {
			std::filesystem::remove_all(downloadDir_/ ".");
		} catch (...) {
			errorCatched(-1);
		}
		progress_ = 100;
		sendDataToBar();
		LoadingBar_->setState(lb::LoadingBar::State::COMPLEET);
		clearFilesEnded();
	}

	void InstalationManager::downloadFile(std::filesystem::path fileName, qint64 tot) {
		downloadFiles({ fileName }, tot);
	}

	void InstalationManager::installFile(std::filesystem::path fileName, qint64 tot, std::filesystem::path dir) {
		installFiles({ fileName }, tot, dir);
	}

	void InstalationManager::downloadFiles(files files, qint64 tot) {
		reset();
		setTotal(tot);
		files_ = files;
		onlyDownload = true;
		ftp_.setFilestoDownload(files_);
		download();
	}

	void InstalationManager::installFiles(files files, qint64 tot, std::filesystem::path dir) {
		reset();
		setTotal(tot);
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

	void InstalationManager::installGame(const cf::Game& game) {
		installFile(game.url.toUtf8().constData(), game.size, game.gameDir.toUtf8().constData());
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
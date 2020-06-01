#include <iostream>
#include <string>
#include "InstalationManager.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include <curl/curl.h>

namespace bb {
	void InstalationManager::download() {
		stage_ = Stage::DOWNLOAD;
		state_ = State::DOWNLOADING;
		visibleState_ = VisibleState::SHOWED;
		stateChanged();
		visibleStateChanged();

		connect(&ftp_, &FtpDownloader::statusSignal, this, &InstalationManager::status);
		connect(&ftp_, &FtpDownloader::ended, this, &InstalationManager::termination);
		connect(&ftp_, &FtpDownloader::error, this, &InstalationManager::errorCatched);
		ftp_.start();
	}
	InstalationManager::~InstalationManager() {
		ftp_.exit();
		ftp_.terminate();
		ftp_.wait();
	}

	void InstalationManager::update() {}
	Q_INVOKABLE std::string InstalationManager::getName() {
		return TYPENAME(InstalationManager);
	}
	Q_INVOKABLE double InstalationManager::getSpeed() const {
		return speed_;
	}
	Q_INVOKABLE int InstalationManager::getState() const {
		return state_;
	}
	Q_INVOKABLE int InstalationManager::getVisibleState() const {
		return visibleState_;
	}
	Q_INVOKABLE bool InstalationManager::getHideLock() const {
		return hideLock_;
	}
	Q_INVOKABLE double InstalationManager::getActual() const {
		return actual_;
	}
	Q_INVOKABLE double InstalationManager::getTotal() const {
		return total_;
	}

	Q_INVOKABLE int InstalationManager::getError() const {
		return Q_INVOKABLE double();
	}

	Q_INVOKABLE QString InstalationManager::getErrorString() const {
		return errorStr_;
	}

	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
	}

	void InstalationManager::status(qint64 progress, qint64 total, double speed) {
		static qint64 lastTotal = 0;
		if (total) {

			actual_ = getMB(progress);
			total_ = getMB(total);
			speed_ = speed; // B/s
		}
		//std::cout << total_ << " \t\t " << progress_ << "\n";
	}
	void InstalationManager::TotalSize(qint64 total) {}

	void InstalationManager::pauseD() {
		ftp_.pause.clear();
		std::cout << "pause\n";
		state_ = State::PAUSE;
		stateChanged();
	}
	void InstalationManager::resumeD() {
		ftp_.resume.clear();
		if (stage_ == Stage::DOWNLOAD)
			state_ = State::DOWNLOADING;
		stateChanged();
	}
	void InstalationManager::stopD() {
		ftp_.stop.clear();
		state_ = State::STOPPED;
		visibleState_ = VisibleState::HIDDEN;
		visibleStateChanged();
		stateChanged();
	}
	void InstalationManager::errorCatched(int code) {
		error_ = code;
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
		state_ = State::ERRORD;
		stateChanged();
		errorChanged();
	}
	void InstalationManager::termination() {
		stage_ = Stage::NONE;
		state_ = State::COMPLEET;
		visibleState_ = VisibleState::HIDDEN;
		stateChanged();
		visibleStateChanged();
		notifyDownload();
		///ftpThread_.wait();
		//ftpThread_.quit();
	}

	void InstalationManager::downloadFile(std::filesystem::path fileName) {
		ftp_.setFilestoDownload({ fileName });
		download();
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
	}
}
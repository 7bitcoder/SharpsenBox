#include <iostream>
#include <string>
#include "LoadingBarManager.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include <curl/curl.h>

namespace bb {
	void LoadingBarManager::download() {
		stage_ = Stage::DOWNLOAD;
		state_ = State::DOWNLOADING;
		visibleState_ = VisibleState::SHOWED;
		stateChanged();
		visibleStateChanged();

		connect(&ftp_, &FtpDownloader::statusSignal, this, &LoadingBarManager::status);
		connect(&ftp_, &FtpDownloader::ended, this, &LoadingBarManager::termination);
		connect(&ftp_, &FtpDownloader::error, this, &LoadingBarManager::errorCatched);
		ftp_.start();
	}
	LoadingBarManager::~LoadingBarManager() {
		ftp_.exit();
		ftp_.terminate();
		ftp_.wait();
	}

	void LoadingBarManager::update() {}
	Q_INVOKABLE std::string LoadingBarManager::getName() {
		return TYPENAME(LoadingBarManager);
	}
	Q_INVOKABLE double LoadingBarManager::getSpeed() const {
		return speed_;
	}
	Q_INVOKABLE int LoadingBarManager::getState() const {
		return state_;
	}
	Q_INVOKABLE int LoadingBarManager::getVisibleState() const {
		return visibleState_;
	}
	Q_INVOKABLE bool LoadingBarManager::getHideLock() const {
		return hideLock_;
	}
	Q_INVOKABLE double LoadingBarManager::getActual() const {
		return actual_;
	}
	Q_INVOKABLE double LoadingBarManager::getTotal() const {
		return total_;
	}

	Q_INVOKABLE int LoadingBarManager::getError() const {
		return Q_INVOKABLE double();
	}

	Q_INVOKABLE QString LoadingBarManager::getErrorString() const {
		return errorStr_;
	}

	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
	}

	void LoadingBarManager::status(qint64 progress, qint64 total, double speed) {
		static qint64 lastTotal = 0;
		if (total) {

			actual_ = getMB(progress);
			total_ = getMB(total);
			speed_ = speed; // B/s
		}
		//std::cout << total_ << " \t\t " << progress_ << "\n";
	}
	void LoadingBarManager::TotalSize(qint64 total) {}

	void LoadingBarManager::pauseD() {
		ftp_.pause.clear();
		std::cout << "pause\n";
		state_ = State::PAUSE;
		stateChanged();
	}
	void LoadingBarManager::resumeD() {
		ftp_.resume.clear();
		if (stage_ == Stage::DOWNLOAD)
			state_ = State::DOWNLOADING;
		stateChanged();
	}
	void LoadingBarManager::stopD() {
		ftp_.stop.clear();
		state_ = State::STOPPED;
		visibleState_ = VisibleState::HIDDEN;
		visibleStateChanged();
		stateChanged();
	}
	void LoadingBarManager::errorCatched(int code) {
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
	void LoadingBarManager::termination() {
		stage_ = Stage::NONE;
		state_ = State::COMPLEET;
		visibleState_ = VisibleState::HIDDEN;
		stateChanged();
		visibleStateChanged();
		notifyDownload();
		///ftpThread_.wait();
		//ftpThread_.quit();
	}

	void LoadingBarManager::downloadFile(std::filesystem::path fileName) {
		ftp_.setFilestoDownload({ fileName });
		download();
	}
	void LoadingBarManager::init() {
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
#include <iostream>
#include <string>
#include "DownloadManager.hpp"
#include "AppBackend.hpp"
#include <curl/curl.h>

namespace bb {
	void DownloadManager::download(bool minimalLock) {
		stage_ = Stage::DOWNLOAD;
		downloadState_ = DownloadState::DOWNLOADING;
		visibleState_ = VisibleState::SHOWED;
		if (minimalLock) {
			hideLock_ = true;
			hideLockChanged();
			visibleState_ = VisibleState::MINIMALIZED;
		}
		downloadStateChanged();
		visibleStateChanged();

		connect(&ftp_, &FtpDownloader::statusSignal, this, &DownloadManager::status);
		connect(&ftp_, &FtpDownloader::ended, this, &DownloadManager::termination);
		connect(&ftp_, &FtpDownloader::error, this, &DownloadManager::errorCatched);
		ftp_.start();
	}
	DownloadManager::~DownloadManager() {
		ftp_.exit();
		ftp_.terminate();
		ftp_.wait();
	}

	void DownloadManager::update() {}
	Q_INVOKABLE std::string DownloadManager::getName() {
		return TYPENAME(DownloadManager);
	}
	Q_INVOKABLE double DownloadManager::getSpeed() const {
		return speed_;
	}
	Q_INVOKABLE int DownloadManager::getDownloadState() const {
		return downloadState_;
	}
	Q_INVOKABLE int DownloadManager::getVisibleState() const {
		return visibleState_;
	}
	Q_INVOKABLE bool DownloadManager::getHideLock() const {
		return hideLock_;
	}
	Q_INVOKABLE double DownloadManager::getActual() const {
		return actual_;
	}
	Q_INVOKABLE double DownloadManager::getTotal() const {
		return total_;
	}

	Q_INVOKABLE int DownloadManager::getError() const {
		return Q_INVOKABLE double();
	}

	Q_INVOKABLE QString DownloadManager::getErrorString() const {
		return errorStr_;
	}

	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
	}

	void DownloadManager::status(qint64 progress, qint64 total, double speed) {
		static qint64 lastTotal = 0;
		if (total) {
			actual_ = getMB(progress);
			total_ = getMB(total);
			speed_ = speed; // B/s
		}
		//std::cout << total_ << " \t\t " << progress_ << "\n";
	}
	void DownloadManager::TotalSize(qint64 total) {}

	void DownloadManager::pauseD() {
		ftp_.pause.clear();
		std::cout << "pause\n";
		downloadState_ = DownloadState::PAUSE;
		downloadStateChanged();
	}
	void DownloadManager::resumeD() {
		ftp_.resume.clear();
		if (stage_ == Stage::DOWNLOAD)
			downloadState_ = DownloadState::DOWNLOADING;
		downloadStateChanged();
	}
	void DownloadManager::stopD() {
		ftp_.stop.clear();
		downloadState_ = DownloadState::STOPPED;
		visibleState_ = VisibleState::HIDDEN;
		visibleStateChanged();
		downloadStateChanged();
	}
	void DownloadManager::errorCatched(int code) {
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
		downloadState_ = DownloadState::ERRORD;
		downloadStateChanged();
		errorChanged();
	}
	void DownloadManager::termination() {
		stage_ = Stage::NONE;
		downloadState_ = DownloadState::COMPLEET;
		visibleState_ = VisibleState::HIDDEN;
		downloadStateChanged();
		visibleStateChanged();
		notifyDownload();
		///ftpThread_.wait();
		//ftpThread_.quit();
	}

	void DownloadManager::downloadLauchBoxJson(std::string url, std::string fileName) {
		ftp_.setFilestoDownload({ {url, downloadDir / fileName } });
		download(true);
	}

	void DownloadManager::downloadFile(std::string url, std::string fileName) {
		ftp_.setFilestoDownload({ {url, downloadDir / fileName } });
		download(false);
	}
	void DownloadManager::init() {
		if (!std::filesystem::exists(downloadDir)) {
			try {
				std::filesystem::create_directory(downloadDir);
			} catch (...) {
				//todo
			}
		}
	}
}
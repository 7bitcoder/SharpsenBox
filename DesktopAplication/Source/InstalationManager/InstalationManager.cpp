#include <iostream>
#include <string>
#include "InstalationManager.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include <curl/curl.h>

namespace bb {
	void InstalationManager::download() {
		stage_ = Stage::DOWNLOAD;
		LoadingBar_->setState( lb::LoadingBar::State::DOWNLOADING );
		LoadingBar_->setVisibleState(lb::LoadingBar::VisibleState::SHOWED );
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
	
	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
	}

	void InstalationManager::status(qint64 progress, qint64 total, double speed) {
		if (total) {
			LoadingBar_->setProgress( getMB(progress) );
			LoadingBar_->setTotal( getMB(total) );
			LoadingBar_->setSpeed( speed ); // B/s
		}
	}
	void InstalationManager::TotalSize(qint64 total) {}

	void InstalationManager::pauseD() {
		ftp_.pause.clear();
		std::cout << "pause\n";
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
		LoadingBar_->setState( lb::LoadingBar::State::ERRORD );
		LoadingBar_->setError(code, errorStr_);
	}
	void InstalationManager::termination() {
		stage_ = Stage::NONE;
		LoadingBar_->setState(lb::LoadingBar::State::COMPLEET );
		LoadingBar_->setVisibleState( lb::LoadingBar::VisibleState::HIDDEN );
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
		LoadingBar_ = &lb::LoadingBar::getObject();
	}
}
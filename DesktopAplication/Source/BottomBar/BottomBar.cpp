#include <iostream>
#include <string>
#include "BottomBar.hpp"
#include "AppBackend.hpp"

namespace bb {
	void BottomBar::download() {
		connect(&ftp_, &FtpDownloader::statusSignal, this, &BottomBar::status);
		connect(&ftp_, &FtpDownloader::ended, this, &BottomBar::termination);
		ftp_.start();
	}
	BottomBar::~BottomBar() {
		ftp_.exit();
		ftp_.terminate();
		ftp_.wait();
	}

	void BottomBar::update() {}
	Q_INVOKABLE std::string BottomBar::getName() {
		return TYPENAME(BottomBar);
	}
	Q_INVOKABLE double BottomBar::getSpeed() const {
		return speed_;
	}
	Q_INVOKABLE int BottomBar::getDownloadState() const {
		return downloadState_;
	}
	Q_INVOKABLE int BottomBar::getVisibleState() const {
		return visibleState_;
	}
	Q_INVOKABLE bool BottomBar::getHideLock() const {
		return hideLock_;
	}
	Q_INVOKABLE double BottomBar::getActual() const {
		return actual_;
	}

	Q_INVOKABLE double BottomBar::getTotal() const {
		return total_;
	}

	namespace {
		double getMB(qint64 progress) {
			double prog = progress / 1024;//B -> KB
			prog /= 1024; //KB -> MB
			return prog;
		}
	}

	void BottomBar::status(qint64 progress, qint64 total, double speed) {
		static qint64 lastTotal = 0;
		if (total) {
			actual_ = getMB(progress);
			total_ = getMB(total);
			speed_ = speed; // B/s
		}
		//std::cout << total_ << " \t\t " << progress_ << "\n";
	}
	void BottomBar::TotalSize(qint64 total) {}

	void BottomBar::pauseD() {
		std::cout << "pause\n";
		ftp_.pause.clear();
	}
	void BottomBar::resumeD() {
		ftp_.resume.clear();
	}
	void BottomBar::stopD() { ftp_.stop.clear(); }
	void BottomBar::termination() {
		///ftpThread_.wait();
		//ftpThread_.quit();
	}
}
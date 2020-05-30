#include "AppUpdateChecker.hpp"
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
#include <QElapsedTimer>

namespace upd {


	//void AppUpdateChecker::checkForUpdates() {
	//	//ftp_.checkForUpdates();
	//}
	void AppUpdateChecker::update() {
		//progress_ = ftp_.getProgress();
		//total_ = ftp_.getTotal();
		std::cout << total_ << " \t\t " << progress_ << "\n";
		//progresChanged();
	}
	std::string AppUpdateChecker::getName() {
		return TYPENAME(AppUpdateChecker);
	}
	//
	//qint64 AppUpdateChecker::getProgress() const {
	//	return progress_;
	//}
}
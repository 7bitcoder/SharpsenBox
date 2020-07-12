#include "AppUpdater.hpp"
#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace upd {

	AppUpdater::AppUpdater() : cf(cf::Config::getObject()), im(im::InstalationManager::getObject()) {
		cf.init();
		im.init();
		auto& downloadDir = cf.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
	}

	void AppUpdater::checkForUpdates() {
		auto& im = im::InstalationManager::getObject();
		connect(&im, &im::InstalationManager::updateStatus, this, &AppUpdater::updateStatus);
		connect(&im, &im::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		connect(&im, &im::InstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		updateStatus(State::DOWNLOADING);
		im.updateMainApp(cf.getVer(), cf.getLauncherAppInfoUrl(), cf.getGameInfoRepository(), cf.getVer() == "0");
	}

	void AppUpdater::updateStatus(State state) {
		state_ = state;
		stateChanged();
	}

	void AppUpdater::updateInstalled(const QString& version) {
		updateStatus(State::ENDED);
		cf.setVer(version);
	}

	void AppUpdater::errorCatched(const QString& what) {
		statusStr_ = what;
		updateStatus(State::ERROR);
	}
}

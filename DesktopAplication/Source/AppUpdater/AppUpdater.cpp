#include "AppUpdater.hpp"
#include "IConfig.hpp"
#include "IInstalationManager.hpp"
#include "ObjectRepo.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace upd {

	AppUpdater::AppUpdater() : cf_(bc::ObjectsRepository::getRepo().getConfig()), im_(bc::ObjectsRepository::getRepo().getInstalationManager()) {
		cf_.init();
		im_.init();
		auto& downloadDir = cf_.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
	}

	void AppUpdater::checkForUpdates() {
		connect(&im_, &im::IInstalationManager::updateStatus, this, &AppUpdater::updateStatus);
		connect(&im_, &im::IInstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		connect(&im_, &im::IInstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		connect(&im_, &im::IInstalationManager::updateProgress, this, &AppUpdater::updateProgress);
		updateStatus(State::DOWNLOADING);
		im_.updateMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository(), cf_.getVer() == "0");
	}

	void AppUpdater::updateStatus(State state) {
		state_ = state;
		stateChanged();
	}

	void AppUpdater::updateInstalled(const QString& version) {
		updateStatus(State::ENDED);
		cf_.setVer(version);
	}

	void AppUpdater::errorCatched(const QString& what) {
		statusStr_ = what;
		updateStatus(State::ERROR);
	}

	void AppUpdater::updateProgress(double progress) {
		progress_ = progress;
		progressChanged();
	}

	Q_INVOKABLE QString AppUpdater::getStateStr() { return statusStr_; }
	Q_INVOKABLE int AppUpdater::getProgress() { return progress_; }
	Q_INVOKABLE int AppUpdater::getState() { return static_cast<int>(state_); }
}

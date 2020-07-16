#include "AppUpdaterManager.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace upd {

	AppUpdaterManager::AppUpdaterManager() : cf_(bc::Component<cf::IConfig>::get()) {
		cf_.init();
		im_.init();
		auto& downloadDir = cf_.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
	}

	void AppUpdaterManager::checkForUpdates() {
		//connect(&im_, &im::IInstalationManager::updateStatus, this, &AppUpdater::updateStatus);
		//connect(&im_, &im::IInstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		//connect(&im_, &im::IInstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		//connect(&im_, &im::IInstalationManager::updateProgress, this, &AppUpdater::updateProgress);
		updateStatus(im::IUpdateManager::State::DOWNLOADING);
		im_.updateMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository(), cf_.getVer() == "0");
	}

	void AppUpdaterManager::updateStatus(im::IUpdateManager::State state) {
		state_ = state;
		stateChanged();
	}

	void AppUpdaterManager::updateInstalled(const QString& version) {
		updateStatus(im::IUpdateManager::State::COMPLEET);
		cf_.setVer(version);
	}

	void AppUpdaterManager::errorCatched(const QString& what) {
		statusStr_ = what;
		updateStatus(im::IUpdateManager::State::ERRORD);
	}

	void AppUpdaterManager::updateProgress(double progress) {
		progress_ = progress;
		progressChanged();
	}

	Q_INVOKABLE QString AppUpdaterManager::getStateStr() { return statusStr_; }
	Q_INVOKABLE int AppUpdaterManager::getProgress() { return progress_; }
	Q_INVOKABLE int AppUpdaterManager::getState() { return static_cast<int>(state_); }
}

#include "AppUpdaterManager.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "UpdateManager.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace sb {

	AppUpdaterManager::AppUpdaterManager() : cf_(Component<IConfig>::get()) {
		cf_.init();
		im_.init();
		auto& downloadDir = cf_.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
	}

	AppUpdaterManager:: ~AppUpdaterManager() {
		im_.terminate();
		im_.wait();
	}

	void AppUpdaterManager::checkForUpdates() {
		connect(&im_, &UpdateManager::setStateLb, this, &AppUpdaterManager::updateSt);
		connect(&im_, &UpdateManager::errorEmit, this, &AppUpdaterManager::errorCatched);
		connect(&im_, &UpdateManager::updateEnded, this, &AppUpdaterManager::updateInstalled);
		connect(&im_, &UpdateManager::updateProgress, this, &AppUpdaterManager::updateProgress);
		updateStatus(IUpdateManager::State::DOWNLOADING);
		if (cf_.getVer() == "0") // needInstallation
			im_.installMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository());
		else //just update
			im_.updateMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository());
	}

	void AppUpdaterManager::updateSt(int state) {
		updateStatus(static_cast<IUpdateManager::State>(state));
	}

	void AppUpdaterManager::updateStatus(IUpdateManager::State state) {
		state_ = static_cast<IUpdateManager::State>(state);
		stateChanged();
	}

	void AppUpdaterManager::updateInstalled(const QString& version) {
		if (!version.isEmpty()) {
			updateStatus(IUpdateManager::State::COMPLEET);
			cf_.setVer(version);
		}
	}

	void AppUpdaterManager::errorCatched(const QString& what) {
		statusStr_ = what;
		updateStatus(IUpdateManager::State::ERRORD);
	}

	void AppUpdaterManager::updateProgress(double progress) {
		progress_ = progress;
		progressChanged();
	}

	Q_INVOKABLE QString AppUpdaterManager::getStateStr() { return statusStr_; }
	Q_INVOKABLE int AppUpdaterManager::getProgress() { return progress_; }
	Q_INVOKABLE int AppUpdaterManager::getState() { return static_cast<int>(state_); }
}

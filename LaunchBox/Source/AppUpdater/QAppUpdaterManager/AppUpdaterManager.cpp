#include "AppUpdaterManager.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "UpdateManager.hpp"
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

	AppUpdaterManager:: ~AppUpdaterManager() {
		im_.terminate();
		im_.wait();
	}

	void AppUpdaterManager::checkForUpdates() {
		connect(&im_, &im::UpdateManager::setStateLb, this, &AppUpdaterManager::updateSt);
		connect(&im_, &im::UpdateManager::errorEmit, this, &AppUpdaterManager::errorCatched);
		connect(&im_, &im::UpdateManager::updateEnded, this, &AppUpdaterManager::updateInstalled);
		connect(&im_, &im::UpdateManager::updateProgress, this, &AppUpdaterManager::updateProgress);
		updateStatus(im::IUpdateManager::State::DOWNLOADING);
		if (cf_.getVer() == "0") // needInstallation
			im_.installMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository());
		else //just update
			im_.updateMainApp(cf_.getVer(), cf_.getLauncherAppInfoUrl(), cf_.getGameInfoRepository());
	}

	void AppUpdaterManager::updateSt(int state) {
		updateStatus(static_cast<im::IUpdateManager::State>(state));
	}

	void AppUpdaterManager::updateStatus(im::IUpdateManager::State state) {
		state_ = static_cast<im::IUpdateManager::State>(state);
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

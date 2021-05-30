#include "AppUpdaterManager.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "UpdateManager.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <string>


namespace sb {

	AppUpdaterManager::AppUpdaterManager() : cf_(Component<IConfig>::Get()) {
		cf_.Init();
		UpdateManager.Init();
		auto& downloadDir = cf_.GetDownloadDir();
		if (!QDir().exists(downloadDir))
			QDir().mkdir(downloadDir);
	}

	AppUpdaterManager:: ~AppUpdaterManager() {
		UpdateManager.terminate();
		UpdateManager.wait();
	}

	void AppUpdaterManager::checkForUpdates() {
		connect(&UpdateManager, &UpdateManager::setStateLb, this, &AppUpdaterManager::updateSt);
		connect(&UpdateManager, &UpdateManager::errorEmit, this, &AppUpdaterManager::errorCatched);
		connect(&UpdateManager, &UpdateManager::updateEnded, this, &AppUpdaterManager::updateInstalled);
		connect(&UpdateManager, &UpdateManager::updateProgress, this, &AppUpdaterManager::updateProgress);
		updateStatus(IUpdateManager::State::DOWNLOADING);
		if (cf_.GetVersion() == "0") // needInstallation
			UpdateManager.installMainApp(cf_.GetVersion(), cf_.GetLauncherAppInfoUrl(), cf_.GetGameInfoRepository());
		else //just Update
			UpdateManager.updateMainApp(cf_.GetVersion(), cf_.GetLauncherAppInfoUrl(), cf_.GetGameInfoRepository());
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
			cf_.SetVersion(version);
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

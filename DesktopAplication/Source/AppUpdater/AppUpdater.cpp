#include "AppUpdater.hpp"
#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace upd {

	AppUpdater::AppUpdater() : cf(cf::Config::getObject()), im(bb::InstalationManager::getObject()) {
		cf.init();
		im.init();
		auto& downloadDir = cf.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
		;//problem
	}

	void AppUpdater::checkForUpdates() {
		auto& im = bb::InstalationManager::getObject();
		connect(&im, &bb::InstalationManager::updateStatus, this, &AppUpdater::updateStatus);
		connect(&im, &bb::InstalationManager::readGameInfo, this, &AppUpdater::readGameInfo);
		connect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		connect(&im, &bb::InstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		state_ = State::downloading;
		im.updateMainApp(cf.getVer(), cf.getLauncherAppInfoUrl(), cf.getGameInfoRepository(), cf.getVer() == "0");
	}

	void AppUpdater::updateStatus(bool needUpdate) {
		if (needUpdate) {
			//download new Laucher
			state_ = State::downloading;
			stateChanged();
		} else {
			state_ = State::noUpdateFound;
			stateChanged();
		}
	}

	void AppUpdater::readGameInfo() {
		state_ = State::updatingGamesInfo;
		stateChanged();
		connect( this, &AppUpdater::parseEnded, &gameParser_, &GameParser::parseEnded);
		gameParser_.parse();
	}

	void AppUpdater::parseEnded() {
		im.updateGamePages(gameParser_.getFiles());
	}

	void AppUpdater::updateInstalled(QString version) {
		state_ = State::ended;
		disconnect(&im, &bb::InstalationManager::updateStatus, this, &AppUpdater::updateStatus);
		disconnect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		disconnect(&im, &bb::InstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		cf.setVer(version);
		gameParser_.updateGamesInfo();
		stateChanged();
	}

	void AppUpdater::errorCatched() {
		//auto msg = bb::InstalationManager::getObject().getErrorString();
		state_ = State::error;
		statusStr_ = im.getError();
		stateChanged();
	}
}

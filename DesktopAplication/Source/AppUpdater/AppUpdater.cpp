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
		connect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		connect(&im, &bb::InstalationManager::updateEnded, this, &AppUpdater::updateInstalled);
		state_ = State::downloading;
		im.downloadFile(cf.getConfigJsonUrl().toStdString(), cf.getConfigJsonFileName().string(), 0);
	}

	void AppUpdater::LauchBoxJsonDownloaded() {
		auto JsonPath = cf.getDownloadDir() / cf.getConfigJsonFileName();
		if (!std::filesystem::exists(JsonPath))
			;//problem
		QString val;
		QFile file;
		file.setFileName(JsonPath.string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		QString version = d["Ver"].toString();
		QString size = d["Size"].toString();
		updateSize_ = std::stoll(size.toUtf8().constData());

		auto& actualVersion = cf.getVer();
		UpdateFile_ = d["FileName"].toString();
		url_ = d["Url"].toString();
		if (version != actualVersion) {
			//download new Laucher
			updatingToVer_ = version;
			state_ = State::downloading;
			stateChanged();
			installUpdate();
		} else {
			state_ = State::noUpdateFound;
			stateChanged();
		}
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
	void AppUpdater::installUpdate() {
		disconnect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::LauchBoxJsonDownloaded);
		connect(&im, &bb::InstalationManager::clearFilesEnded, this, &AppUpdater::updateInstalled);
		im.installFile(url_.toStdString(), UpdateFile_.toStdString(), updateSize_);
	}


	void AppUpdater::updateInstalled() {
		state_ = State::ended;
		disconnect(&im, &bb::InstalationManager::clearFilesEnded, this, &AppUpdater::updateInstalled);
		disconnect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::updateDownloaded);
		disconnect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		cf.setVer(updatingToVer_);
		stateChanged();
	}

	void AppUpdater::errorCatched() {
		//auto msg = bb::InstalationManager::getObject().getErrorString();
		state_ = State::error;
		statusStr_ = im.getError();
		stateChanged();
	}
}

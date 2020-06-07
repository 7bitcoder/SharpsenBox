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
		auto& cfJson = cf.getConfigJson();
		auto& im = bb::InstalationManager::getObject();
		connect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::LauchBoxJsonDownloaded);
		connect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		state_ = State::downloading;
		im.downloadFile(cfJson, 0);
	}

	void AppUpdater::LauchBoxJsonDownloaded() {
		auto& downloadDIr = cf.getDownloadDir();
		auto JsonPath = downloadDIr / LBJsonFileName;
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
		UpdateFile = d["Url"].toString();
		if (version != actualVersion) {
			//download new Laucher
			state_ = State::downloading;
			statusStr_ = "Downloading update";
			stateStrChanged();
			stateChanged();
			installUpdate();
		} else {
			state_ = State::noUpdateFound;
			statusStr_ = "No updates found";
			stateStrChanged();
			stateChanged();
		}

	}

	void AppUpdater::installUpdate() {
		disconnect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::LauchBoxJsonDownloaded);
		connect(&im, &bb::InstalationManager::clearFilesEnded, this, &AppUpdater::updateInstalled);
		connect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::updateDownloaded);
		im.installFile(UpdateFile.toUtf8().constData(), updateSize_);
	}

	void AppUpdater::updateDownloaded() {
		state_ = State::installing;
		statusStr_ = "Installing update";
		stateStrChanged();
		stateChanged();
	}

	void AppUpdater::updateInstalled() {
		state_ = State::ended;
		statusStr_ = "Installation Complete";
		stateStrChanged();
		disconnect(&im, &bb::InstalationManager::clearFilesEnded, this, &AppUpdater::updateInstalled);
		disconnect(&im, &bb::InstalationManager::downloadEnded, this, &AppUpdater::updateDownloaded);
		disconnect(&im, &bb::InstalationManager::errorEmit, this, &AppUpdater::errorCatched);
		stateChanged();
	}

	void AppUpdater::errorCatched() {
		//auto msg = bb::InstalationManager::getObject().getErrorString();
		state_ = State::error;
		statusStr_ = im.getError();
		stateChanged();
	}
}
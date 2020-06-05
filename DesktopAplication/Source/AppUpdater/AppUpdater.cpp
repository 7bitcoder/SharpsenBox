#include "AppUpdater.hpp"
#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <string>


namespace upd {

	AppUpdater::AppUpdater() {
		auto& cf = cf::Config::getObject();
		cf.init();
		auto& downloadDir = cf.getDownloadDir();
		if (!std::filesystem::exists(downloadDir))
			std::filesystem::create_directory(downloadDir);
		;//problem
	}

	void AppUpdater::checkForUpdates() {
		auto& cfJson = cf::Config::getObject().getConfigJson();
		auto& im = bb::InstalationManager::getObject();
		connect(&im, &bb::InstalationManager::notifyDownload, this, &AppUpdater::LauchBoxJsonDownloaded);
		connect(&im, &bb::InstalationManager::errorCatched, this, &AppUpdater::errorCatched);
		im.downloadFile(cfJson);
	}

	void AppUpdater::LauchBoxJsonDownloaded() {
		auto& dm = bb::InstalationManager::getObject();
		auto& downloadDIr = cf::Config::getObject().getDownloadDir();
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
		auto& actualVersion = cf::Config::getObject().getVer();
		if (version != actualVersion) {
			//download new Laucher
			std::cout << "downloading Laucher\n";
			state_ = State::downloading;
			statusStr_ = "Downloading update";
			stateStrChanged();
			downloadUpdate();
		} else {
			state_ = State::noUpdateFound;
			statusStr_ = "No updates found";
			stateStrChanged();
		}

	}

	void AppUpdater::downloadUpdate() {
		auto& im = bb::InstalationManager::getObject();
		disconnect(&im, &bb::InstalationManager::notifyDownload, this, &AppUpdater::LauchBoxJsonDownloaded);
		connect(&im, &bb::InstalationManager::notifyDownload, this, &AppUpdater::updateDownloaded);
		im.downloadFile("./bin_.rar");
	}

	void AppUpdater::updateDownloaded() {
		state_ = State::installing;
		statusStr_ = "Installing update";
		stateStrChanged();
	}
	void AppUpdater::errorCatched() {
		if (state_ == State::downloading) { //handle downloading errors
			//auto msg = bb::InstalationManager::getObject().getErrorString();
			state_ = State::error;
			//statusStr_ = msg;
			stateStrChanged();
		} else if (state_ == State::installing) {//todo
		}
	}
}
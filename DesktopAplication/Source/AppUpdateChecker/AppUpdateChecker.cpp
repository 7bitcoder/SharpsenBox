#include "AppUpdateChecker.hpp"
#include "Config.hpp"
#include <QElapsedTimer>
#include "DownloadManager.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


namespace upd {


	void AppUpdateChecker::checkForUpdates() {
		auto& dm = bb::DownloadManager::getObject();
		connect(&dm, &bb::DownloadManager::notifyDownload, this, &AppUpdateChecker::LauchBoxJsonDownloaded);
		dm.downloadLauchBoxJson(LBJsonUrl, LBJsonFileName);
	}
	void AppUpdateChecker::LauchBoxJsonDownloaded() {
		auto& dm = bb::DownloadManager::getObject();
		auto JsonPath = dm.getDownloadDir() / LBJsonFileName;
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
		if (version != cf::Config::getObject().getVer()) {
			//download new Laucher
			std::cout << "downloading Laucher\n";
		}

	}
	void AppUpdateChecker::update() {
		//progress_ = ftp_.getProgress();
		//total_ = ftp_.getTotal();
		std::cout << total_ << " \t\t " << progress_ << "\n";
		//progresChanged();
	}
	std::string AppUpdateChecker::getName() {
		return TYPENAME(AppUpdateChecker);
	}

	void AppUpdateChecker::init() {
		checkForUpdates();
	}
}
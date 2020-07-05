#include "AppInfoParser.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace bb {
	void AppInfoParser::parse(bool fullInstall) {
		fullInstall_ = fullInstall;
		start();
	}
	
	void AppInfoParser::run() {
		QString val;
		QFile file;
		//open LaunchBoxConfig file
		auto path = cf::Config::getObject().getDownloadDir() / parseInfoFileName;
		file.setFileName(path.generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		auto& ghj = val.toStdString();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		versionToUpdate_ = d["Ver"].toString();
		fileListUrl_ = d["FileList"].toString();
		auto& ss = versionToUpdate_.toStdString();
		auto& gg = actualVersion_.toStdString();
		pathFiles_.push_back({ fileListUrl_.toStdString(), "FileList.json" });
		if (fullInstall_) {
			needUpdate_ = true;
		} else if ( versionToUpdate_ != actualVersion_ ) { //need update
			needUpdate_ = true;
			getPathUrls(d["Versioning"].toObject());
		} else { //app is up to date
			needUpdate_ = false;
		}
		parseEnded();
	}
	void AppInfoParser::getPathUrls(QJsonObject& pathList) {
		for (auto it = pathList.begin(); it != pathList.end(); it++) {
			auto& ff = it.key().toStdString();
			auto& ac = actualVersion_.toStdString();
			if (it.key() <= actualVersion_)
				continue;
			pathFiles_.push_back({ it->toString().toStdString() , "Path-" + it.key().toStdString() + ".json"});
		}
	}

	void AppInfoParser::reset() {
		actualVersion_ = "";
		needUpdate_ = false;
		fileListUrl_ = "";
		pathFiles_.clear();
	}
}
#include "AppInfoParser.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace bb {
	void AppInfoParser::parse() {
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
		QString ver = d["Ver"].toString();
		auto& ss = ver.toStdString();
		auto& gg = actualVersion_.toStdString();
		if (ver != actualVersion_) { //need update
			needUpdate_ = true;
			QJsonObject appFiles = d["AppFiles"].toObject();
			for (auto& key : appFiles.keys()) {
				QJsonObject value = appFiles[key].toObject();
				totalBytesTo_ += std::stoll(value["Size"].toString().toStdString());
				std::filesystem::path url = value["Url"].toString().toStdString();
				files_.push_back({ url, key.toStdString() });
			}
			actualVersion_ = ver;
		} else { //app is up to date
			needUpdate_ = false;
		}
		parseEnded();
	}

	void AppInfoParser::reset() {
		actualVersion_ = "";
		files_.clear();
		totalBytesTo_ = 0;
		needUpdate_ = false;
	}
}
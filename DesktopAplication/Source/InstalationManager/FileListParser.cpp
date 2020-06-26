#include "FileListParser.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace bb {
	void FileListParser::parse() {
		start();
	}
	
	void FileListParser::run() {
		QString val;
		QFile file;
		//open LaunchBoxConfig file
		auto path = cf::Config::getObject().getDownloadDir() / parseInfoFileName;
		file.setFileName(path.generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		auto& ghj = val.toStdString();
		file.close();
		fileList_ = QJsonDocument::fromJson(val.toUtf8());
		QString ver = fileList_["Ver"].toString();
		auto& ss = ver.toStdString();
		auto& gg = actualVersion_.toStdString();
		if (ver != toUpdateVersion_) { //need update
			//todo error
		} else { //app is up to date
			readPackets
		}
		parseEnded();
	}

	void FileListParser::getAllFiles(QJsonDocument& doc) {
		QJsonObject appFiles = doc["AppFiles"].toObject();
		for (auto& key : appFiles.keys()) {
			QJsonObject value = appFiles[key].toObject();
			qint64 size = std::stoll(value["Size"].toString().toStdString());
			QString url = value["Url"].toString();
			if (allFiles_.contains(key))
				throw std::exception("Files Duplicated");
			allFiles_.insert({ key, {url , size} });
		}
	}

	void FileListParser::getDeltaFiles(QJsonDocument& doc) {

	}

	void FileListParser::reset() {
		actualVersion_ = "";
		files_.clear();
		totalBytesTo_ = 0;
		needUpdate_ = false;
	}
}
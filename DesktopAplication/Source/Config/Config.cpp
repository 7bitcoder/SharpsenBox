#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <iostream>

namespace cf {

	Config::Config() {
		if (!std::filesystem::exists(configJson_))
			;//problem
		QString val;
		QFile file;
		file.setFileName((".." / configJson_).string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		version_ = d["Ver"].toString();
		std::cout << version_.toUtf8().constData();
		auto games = d["Games"].toArray();
		for (int i = 0; i < games.size(); ++i) {
			auto gam = readGameInfo((games[i].toString() + ".json").toUtf8().constData());
			games_.insert({ gam.id, gam });
		}
	}

	Game Config::readGameInfo(std::filesystem::path path) {
		QFile file;
		file.setFileName((".." / path).string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QString val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		Game g;
		g.id = d["Id"].toInt();
		g.version = d["Ver"].toString();
		QString size = d["Size"].toString();
		g.size = std::stoll(size.toUtf8().constData());
		g.url = d["Url"].toString();
		g.installed = d["Url"].toBool();
	}	

	void Config::init() {
	}

	std::string Config::getName() {
		return TYPENAME(Config);
	}

	Config::~Config() {
		// save config
	}
	//
	//qint64 AppUpdateChecker::getProgress() const {
	//	return progress_;
	//}
}
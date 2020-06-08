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

	Game Config::readGameInfo(std::filesystem::path path) 
	{
		QFile file;
		file.setFileName((".." / path).string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QString val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		Game g;
		auto ss = std::string(d["Id"].toString().toUtf8().constData());
		g.id = std::stoi(ss);
		g.installed = d["Url"].toBool();
		g.version = d["Ver"].toString();
		QString size = d["Size"].toString();
		g.size = std::stoll(size.toUtf8().constData());
		g.url = d["Url"].toString();
		g.gameDir = d["GameDir"].toString();
		g.execDir = d["GameExecPath"].toString();
		g.shortcutPath = d["ShortcutPath"].toString();
		g.shortcutPath = d["Shortcut"].toBool();
		//std::cout << g.id << g.url.toUtf8().constData() << g.version.toUtf8().constData() << g.installed;
		return g;
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

	Q_INVOKABLE bool Config::installed(int id) const {
		auto it = games_.find(id);
		if (it != games_.end())
			return it->second.installed;
		return false;
	}
	Q_INVOKABLE QString Config::gamePath(int id) const {
		auto it = games_.find(id);
		if (it != games_.end()) {
			auto& path = it->second.gameDir;
			if (path.isEmpty()) {
				std::string pf("file:///");
				std::string  env(getenv("PROGRAMFILES"));
				if (!env.empty()) { //windows
					std::filesystem::path path = env;
					pf += path.generic_string();
					return pf.c_str();
				} else { //mac /linux
					return "";
				}
			}
		}
		return "";
	}
	Q_INVOKABLE void Config::setGamevariables(int id, QString path, bool shortcut) {
		auto it = games_.find(id);
		if (it != games_.end()) {
			auto game = it->second;
			game.gameDir = path;
			game.shortcut = shortcut;
			games_.insert({ game.id, game });
		}
	}
}
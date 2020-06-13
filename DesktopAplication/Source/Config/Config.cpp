#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <iostream>

namespace cf {

	namespace {
		bool readBool(const QJsonValue& val) { return val.toString() == "1"; }
		QString writeBool(bool f) { return f ? "1" : "0"; }
	}

	Config::Config() {
		if (!std::filesystem::exists(config_))
			std::filesystem::create_directories(config_);
		if (!std::filesystem::exists(getConfigJson()))
			;//problem
		QString val;
		QFile file;
		file.setFileName(getConfigJson().generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		version_ = d["Ver"].toString();
		std::cout << version_.toUtf8().constData();
		auto games = d["Games"].toArray();
		downloadSpeed_ = std::stoi(d["DownloadSpeed"].toString().toStdString());
		for (int i = 0; i < games.size(); ++i) {
			auto gam = readGameInfo((games[i].toString() + ".json").toUtf8().constData());
			games_.insert({ gam.id, gam });
		}
	}

	Game Config::readGameInfo(std::filesystem::path path) {
		QFile file;
		file.setFileName((config_ / path).string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QString val = file.readAll();
		std::string gg(val.toUtf8().constData());
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		Game g;
		auto ss = std::string(d["Id"].toString().toUtf8().constData());
		g.id = std::stoi(ss);
		g.installed = readBool(d["Installed"]);
		g.version = d["Ver"].toString();
		g.name = d["Name"].toString();
		QString size = d["Size"].toString();
		g.size = std::stoll(size.toUtf8().constData());
		g.url = d["Url"].toString();
		g.gameDir = d["GameDir"].toString();
		g.execDir = d["GameExecPath"].toString();
		g.shortcutPath = d["ShortcutPath"].toString();
		g.shortcut = readBool(d["Shortcut"]);
		g.autoCheck = readBool(d["AutoUpdate"]);
		g.fileName = d["FileName"].toString();
		//std::cout << g.id << g.url.toUtf8().constData() << g.version.toUtf8().constData() << g.installed;
		return g;
	}

	void Config::writedGameInfo(Game& game) {
		QJsonDocument d;
		QJsonObject RootObject = d.object();
		RootObject.insert("Id", QString::number(game.id));
		RootObject.insert("Name", game.name);
		RootObject.insert("Installed", writeBool(game.installed));
		RootObject.insert("Ver", game.version);
		RootObject.insert("Size", QString::number(game.size));
		RootObject.insert("Url", game.url);
		RootObject.insert("FileName", game.fileName);
		RootObject.insert("GameDir", game.gameDir);
		RootObject.insert("GameExecPath", game.execDir);
		RootObject.insert("ShortcutPath", game.shortcutPath);
		RootObject.insert("Shortcut", writeBool(game.shortcut));
		RootObject.insert("AutoUpdate", writeBool(game.autoCheck));

		d.setObject(RootObject);
		QFile file;
		std::string ff(game.name.toUtf8().constData());
		auto gamePath = config_ / (game.name.toStdString() + ".json");
		file.setFileName(gamePath.generic_string().c_str());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();
	}

	void Config::init() {}

	std::string Config::getName() {
		return TYPENAME(Config);
	}

	Config::~Config() {
		QJsonDocument d;
		QJsonObject RootObject = d.object();
		RootObject.insert("Ver", version_);
		RootObject.insert("DownloadSpeed", QString::number(downloadSpeed_));
		QJsonArray arr;
		for (auto& game : games_) {
			writedGameInfo(game.second);
			arr.push_back(game.second.name);
		}
		RootObject.insert("Games", arr);
		d.setObject(RootObject);
		QFile file;
		file.setFileName(getConfigJson().generic_string().c_str());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();
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

	Q_INVOKABLE QUrl Config::defaultInstallDir() {
		if (getenv("PROGRAMFILES")) { //windows
			std::filesystem::path path = getenv("PROGRAMFILES");
			return QUrl::fromLocalFile(path.generic_string().c_str());
		} else { //mac /linux
			return "";
		}
		return "";
	}


	Game& Config::getGame(int id) {
		if (games_.contains(id)) {
			return games_[id];
		}
		throw std::exception("Bad game Id");
	}
}

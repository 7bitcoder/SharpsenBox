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
		//open LaunchBoxConfig file
		file.setFileName(getConfigJson().generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		auto& ff = val.toStdString();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

		// Read settings
		version_ = d["Ver"].toString();
		downloadSpeed_ = std::stoi(d["DownloadSpeed"].toString().toStdString());
		gameInfoRepo_ = d["GamesInfoRepository"].toString().toStdString();
		LauncherAppInfo = d["AppInfoUrl"].toString().toStdString();

		std::cout << "version: " << version_.toUtf8().constData();
		readGames();
	}
	void Config::readGames() {
		QString val;
		QFile file;
		//open LaunchBoxConfig file
		file.setFileName((config_ / gamesFileName_).generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();

		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject json = d.object();
		for (auto& key : json.keys()) {
			QJsonObject value = json[key].toObject();
			auto game = readGameInfo(value);
			game.name = key;
			games_.insert({ game.id, game });
		}
	}
	Game Config::readGameInfo(const QJsonObject& value) {
		Game g;
		auto ss = std::string(value["Id"].toString().toUtf8().constData());
		g.id = std::stoi(ss);
		g.installed = readBool(value["Installed"]);
		g.shortcut = readBool(value["Shortcut"]);
		g.autoCheck = readBool(value["AutoUpdate"]);
		g.version = value["Ver"].toString();
		g.appInfoUrl = value["AppInfoUrl"].toString();
		g.gameDir = value["GameDir"].toString();
		g.execPath = value["GameExecPath"].toString();
		g.shortcutPath = value["ShortcutPath"].toString();
		g.desctiption = value["Desctiption"].toString();
		g.youtubeLink = value["YoutubeLink"].toString();
		g.additionalInfo = value["AdditionalInfo"].toString();
		//std::cout << g.id << g.url.toUtf8().constData() << g.version.toUtf8().constData() << g.installed;
		return g;
	}

	void Config::writeGames() {
		QJsonObject RootObject;
		for (auto& game : games_) {
			auto jsonParsed = parseGameInfo(game.second);
			RootObject.insert(game.second.name, jsonParsed);
		}
		QFile file;
		QJsonDocument d;
		d.setObject(RootObject);
		file.setFileName((config_ / gamesFileName_).generic_string().c_str());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();
	}

	QJsonObject Config::parseGameInfo(const Game& game) {
		QJsonObject RootObject;
		RootObject.insert("Id", QString::number(game.id));
		RootObject.insert("Ver", game.version);
		RootObject.insert("AutoUpdate", writeBool(game.autoCheck));
		RootObject.insert("Installed", writeBool(game.installed));
		RootObject.insert("Shortcut", writeBool(game.shortcut));
		RootObject.insert("GameDir", game.gameDir);
		RootObject.insert("GameExecPath", game.execPath);
		RootObject.insert("ShortcutPath", game.shortcutPath);
		RootObject.insert("Desctiption", game.desctiption);
		RootObject.insert("YoutubeLink", game.youtubeLink);
		RootObject.insert("AdditionalInfo", game.additionalInfo);
		RootObject.insert("AppInfoUrl", game.appInfoUrl);
		return RootObject;
	}

	void Config::init() {}

	std::string Config::getName() {
		return TYPENAME(Config);
	}

	Config::~Config() {
		QFile file;

		QJsonDocument d;
		QJsonObject RootObject = d.object();


		RootObject.insert("Ver", version_);
		RootObject.insert("DownloadSpeed", QString::number(downloadSpeed_));
		RootObject.insert("GamesInfoRepository", gameInfoRepo_.string().c_str());
		RootObject.insert("AppInfoUrl", LauncherAppInfo.string().c_str());

		d.setObject(RootObject);
		file.setFileName(getConfigJson().generic_string().c_str());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();

		writeGames();
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

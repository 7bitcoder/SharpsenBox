#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <iostream>
#include <algorithm>
#include <QDir>

namespace bc {
	cf::IConfig* bc::Get< cf::IConfig >::component_ = nullptr;
}
namespace cf {

	namespace {
		bool readBool(const QJsonValue& val) { return val.toString() == "1"; }
		QString writeBool(bool f) { return f ? "1" : "0"; }
	}

	Config::Config() :bc::Get<IConfig>(this) {
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

		for (auto& keys : games_) {
			sortedId_.push_back(keys.first);
		}
		std::sort(sortedId_.begin(), sortedId_.end(), std::less<int>());
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
		g.presentationUrl = value["PresentationUrl"].toString();
		g.PresentationQml = value["PresentationQml"].toString();
		g.PresentationPackUrl = value["PresentationPackUrl"].toString();
		g.PresentationVer = value["PresentationVer"].toString();
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
		RootObject.insert("PresentationUrl", game.presentationUrl);

		RootObject.insert("PresentationQml", game.PresentationQml);
		RootObject.insert("PresentationPackUrl", game.PresentationPackUrl);
		RootObject.insert("PresentationVer", game.PresentationVer);
		RootObject.insert("AppInfoUrl", game.appInfoUrl);
		return RootObject;
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

	Q_INVOKABLE bool Config::installed(int id) {
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

	bool Config::gameExists(int id) {
		return games_.contains(id);
	}

	Q_INVOKABLE QString Config::gameInfoDir(int id) {
		return getCurrentDirectory() + "/Games/" + getGame(id).name + "/";
	}

	std::string Config::gamePageDir(int id) {
		return std::string("../Games/") + getGame(id).name.toStdString();
	}

	Q_INVOKABLE int Config::getGameId() {
		static int i = 0;
		if (i < maxGameBarLen_ && i < sortedId_.size()) {
			int id = sortedId_.at(i++);
			return id;
		}
		i = 0;
		return 0; // end
	}

	Q_INVOKABLE QString Config::getCurrentDirectory() { return "file:///" + QDir::currentPath() + "/.."; }

	Q_INVOKABLE QString Config::getGamePresentationUrl(int id) {
		auto& game = getGame(id);
		return game.presentationUrl;
	}

	Q_INVOKABLE QString Config::getPresentationFile(int id) {
		auto& game = getGame(id);
		auto& url = game.presentationUrl;
		if (!url.isEmpty()) { // its http link or locak html file
			return "WebGamePage.qml";
		} else { // local qml file 
			auto full = gameInfoDir(game.id) + game.PresentationQml;
			return full;
		}
	}

	Q_INVOKABLE int Config::getDefaultGameId() {
		if (sortedId_.size())
			return sortedId_.at(0);
		throw std::exception("None games provided");
	}
}

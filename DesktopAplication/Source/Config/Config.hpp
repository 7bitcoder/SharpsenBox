#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include <filesystem>
#include <vector>


namespace cf {
	class Game {
	public:
		int id;
		bool installed;
		bool shortcut;
		bool autoCheck;
		size_t size;
		QString name;
		QString version;
		QString url;
		QString fileName;
		QString gameDir;
		QString execDir;
		QString shortcutPath;
		std::unordered_map<QString, int> sha;
		bool operator==(const Game& g) {
			return id == g.id;
		}
	};

	class Config : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Config& getObject() {
			static Config uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override {};
		void init() override;
		std::string getName() override;

		// interface 
		Game& getGame(int id);
		QString& getVer() { return version_; }
		void setVer(QString ver) { version_ = ver; }
		std::filesystem::path& getDownloadDir() { return downloadDir_; }
		std::filesystem::path getConfigJson() { return config_ / configJson_; }
		std::filesystem::path getConfigJsonFileName() { return configJson_; }

		//QMl invoklabes
		Q_INVOKABLE bool installed(int id) const;
		Q_INVOKABLE QUrl defaultInstallDir();
		Q_INVOKABLE void setDownloadSpeed(qint32 dp) { downloadSpeed_ = dp; }
		Q_INVOKABLE qint32 getDownloadSpeed() { return downloadSpeed_; }
		Q_INVOKABLE void setGameAutoCheck(int id, bool val) { getGame(id).autoCheck = val; }
		Q_INVOKABLE bool getGameAutoCheck(int id) { bool v = getGame(id).autoCheck;	return v; }
		Q_INVOKABLE QString getGameName(int id) { return getGame(id).name; }
		Q_INVOKABLE QString  getConfigJsonUrl() { return configJsonUrl_; }
	private:
		Config();
		virtual ~Config();
		Game readGameInfo(std::filesystem::path path);
		void writedGameInfo(Game& game);
	private:
		QString version_;
		std::filesystem::path config_ = "../Config";
		std::filesystem::path configJson_ = "LaunchBoxInfo.json";
		QString configJsonUrl_ = "https://sourceforge.net/projects/gamelaunchbox/files/LaunchBoxInfo.json";

		std::filesystem::path downloadDir_ = "../Download";
		std::unordered_map<int, Game> games_;
		qint32 downloadSpeed_;
	};
}

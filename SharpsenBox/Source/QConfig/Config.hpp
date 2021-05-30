#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "Game.hpp"
#include "IConfig.hpp"
#include <filesystem>
#include <vector>


namespace sb {
	class Config final : public IConfig {
		Q_OBJECT
	public:
		Config();
		virtual ~Config();

		// implementation IQmlObject
		void update() final {};
		void init() final {};
		std::string getName() final { return TYPENAME(Config); }

		// interface 
		void insertGame(Game& game) final { games_.insert({ game.id,  game }); };
		bool gameExists(int id) final;
		Game& getGame(int id) final;
		std::string gamePageDir(int id) final;
		QString& getVer()final { return version_; }
		void setVer(QString ver) final { version_ = ver; }

		std::filesystem::path& getDownloadDir() final { return downloadDir_; }
		std::filesystem::path getConfigJson() final { return config_ / configJson_; }
		std::filesystem::path getConfigJsonFileName() final { return configJson_; }
		std::filesystem::path getLauncherAppInfoUrl() final { return LauncherAppInfo; }
		std::filesystem::path getGameInfoRepository() final { return gameInfoRepo_; }


		Q_INVOKABLE bool installed(int id);
		Q_INVOKABLE QString gameInfoDir(int id);
		Q_INVOKABLE QUrl defaultInstallDir();
		Q_INVOKABLE void setDownloadSpeed(qint32 dp) { downloadSpeed_ = dp; }
		Q_INVOKABLE qint32 getDownloadSpeed() { return downloadSpeed_; }
		Q_INVOKABLE void setGameAutoCheck(int id, bool val) { getGame(id).autoCheck = val; }
		Q_INVOKABLE bool getGameAutoCheck(int id) { bool v = getGame(id).autoCheck;	return v; }
		Q_INVOKABLE QString getGameName(int id) { return getGame(id).name; }
		Q_INVOKABLE QString  getConfigJsonUrl() { return ""; } //!!!!!!!!!
		Q_INVOKABLE int getGameId(); // get game id for game choser
		Q_INVOKABLE QString getGamePresentationUrl(int id); // get game id for game choser
		Q_INVOKABLE QString getPresentationFile(int id);
		Q_INVOKABLE int getDefaultGameId();
		Q_INVOKABLE QString getCurrentDirectory();


	private:
		void readGames();
		Game readGameInfo(const QJsonObject& value);
		void writeGames();
		QJsonObject parseGameInfo(const Game& game);
	private:
		std::filesystem::path config_ = "../Config";
		std::filesystem::path downloadDir_ = "../Download";
		std::filesystem::path configJson_ = "SharpsenBoxInfo.json";
		std::filesystem::path gamesFileName_ = "Games.json";

		std::unordered_map<int, Game> games_;

		QString version_;
		qint32 downloadSpeed_;
		std::filesystem::path gameInfoRepo_;
		std::filesystem::path LauncherAppInfo;

		//for gamebar
		int maxGameBarLen_ = 10;
		std::vector<int> sortedId_;
	};
}

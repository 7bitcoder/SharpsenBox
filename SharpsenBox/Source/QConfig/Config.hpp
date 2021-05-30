#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "Game.hpp"
#include "IConfig.hpp"

namespace sb
{
	class Config final : public IConfig
	{
		Q_OBJECT
	public:
		Config();
		virtual ~Config();

		QString CombinePath(const QList<QString> paths) final;
		// implementation IQmlObject
		void Update() final;
		void Init() final;
		std::string GetName() final;

		QString GamePageDir(int id);

		// interface
		void AddNewGame(Game &game) final;
		bool GameExists(int id) final;
		Game &GetGame(int id) final;

		QString &GetVersion() final;
		void SetVersion(QString ver) final;

		QString &GetDownloadDir() final;
		QString GetConfigJsonFilePath() final;
		QString GetConfigJsonFileName() final;
		QString GetLauncherAppInfoUrl() final;
		QString GetGameInfoRepository() final;

		Q_INVOKABLE QUrl DefaultInstallDir();

		Q_INVOKABLE void SetDownloadSpeed(qint32 dp);
		Q_INVOKABLE qint32 GetDownloadSpeed();

		Q_INVOKABLE void SetGameUpdateAutoCheck(int id, bool val);
		Q_INVOKABLE bool GetGameUpdateAutoCheck(int id);

		Q_INVOKABLE bool IsGameInstalled(int id);
		Q_INVOKABLE QString GameInfoDir(int id);
		Q_INVOKABLE QString GetGameTitle(int id);
		Q_INVOKABLE QString GetConfigJsonUrl();
		Q_INVOKABLE int GetGameId();						// get game id for game choser
		Q_INVOKABLE QString GetGamePresentationUrl(int id); // get game id for game choser
		Q_INVOKABLE QString GetPresentationFile(int id);
		Q_INVOKABLE int GetDefaultGameId();
		Q_INVOKABLE QString GetCurrentDirectory();

	private:
		void ReadGames();
		Game ReadGameInfo(const QJsonObject &value);
		void WriteGames();
		QJsonObject ParseGameInfo(const Game &game);

	private:
		QString _Config = "../Config";
		QString _DownloadDir = "../Download";
		QString _ConfigJson = "SharpsenBoxInfo.json";
		QString _GamesFileName = "Games.json";

		QMap<int, Game> _Games;

		QString _Version;
		qint32 _DownloadSpeed;
		QString _GameInfoRepo;
		QString _LauncherAppInfo;

		//for gamebar
		int _MaxGameBarLen = 10;
		QVector<int> _SortedId;
	};
}

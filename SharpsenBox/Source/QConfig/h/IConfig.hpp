#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IComponent.hpp"
#include <filesystem>
#include <vector>

namespace sb
{
	class Config;
	struct Game;

	struct IConfig : public IComponent
	{
	public:
		virtual ~IConfig(){};

		// interface
		virtual QString CombinePath(const QList<QString> paths) = 0;

		virtual void AddNewGame(Game &game) = 0;
		virtual bool GameExists(int id) = 0;
		virtual Game &GetGame(int id) = 0;
		virtual QString GamePageDir(int id) = 0;

		virtual QString &GetVersion() = 0;
		virtual void SetVersion(QString ver) = 0;

		virtual QString &GetDownloadDir() = 0;
		virtual QString GetConfigJsonFilePath() = 0;
		virtual QString GetConfigJsonFileName() = 0;
		virtual QString GetLauncherAppInfoUrl() = 0;
		virtual QString GetGameInfoRepository() = 0;

		virtual Q_INVOKABLE bool IsGameInstalled(int id) = 0;
		virtual Q_INVOKABLE QString GameInfoDir(int id) = 0;
		virtual Q_INVOKABLE QUrl DefaultInstallDir() = 0;
		virtual Q_INVOKABLE void SetDownloadSpeed(qint32 dp) = 0;
		virtual Q_INVOKABLE qint32 GetDownloadSpeed() = 0;
		virtual Q_INVOKABLE void SetGameUpdateAutoCheck(int id, bool val) = 0;
		virtual Q_INVOKABLE bool GetGameUpdateAutoCheck(int id) = 0;
		virtual Q_INVOKABLE QString GetGameTitle(int id) = 0;
		virtual Q_INVOKABLE QString GetConfigJsonUrl() = 0;
		virtual Q_INVOKABLE int GetGameId() = 0;
		virtual Q_INVOKABLE QString GetGamePresentationUrl(int id) = 0;
		virtual Q_INVOKABLE QString GetPresentationFile(int id) = 0;
		virtual Q_INVOKABLE int GetDefaultGameId() = 0;
		virtual Q_INVOKABLE QString GetCurrentDirectory() = 0;
	};
}

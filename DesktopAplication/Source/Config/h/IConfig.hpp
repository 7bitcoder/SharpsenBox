#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IComponent.hpp"
#include <filesystem>
#include <vector>

namespace cf {
	class Config;
	class Game;

	struct IConfig : public  bc::IQmlComponent {
		Q_OBJECT
	public:
		virtual ~IConfig() {};

		// interface 
		virtual void insertGame(Game& game) = 0;
		virtual bool gameExists(int id) = 0;
		virtual Game& getGame(int id) = 0;
		virtual std::string gamePageDir(int id) = 0;
		virtual QString& getVer() = 0;
		virtual void setVer(QString ver) = 0;

		virtual std::filesystem::path& getDownloadDir() = 0;
		virtual std::filesystem::path getConfigJson() = 0;
		virtual std::filesystem::path getConfigJsonFileName() = 0;
		virtual std::filesystem::path getLauncherAppInfoUrl() = 0;
		virtual std::filesystem::path getGameInfoRepository() = 0;

		//QMl invoklabes
		Q_INVOKABLE virtual bool installed(int id) = 0;
		Q_INVOKABLE virtual QString gameInfoDir(int id) = 0;
		Q_INVOKABLE virtual QUrl defaultInstallDir() = 0;
		Q_INVOKABLE virtual void setDownloadSpeed(qint32 dp) = 0;
		Q_INVOKABLE virtual qint32 getDownloadSpeed() = 0;
		Q_INVOKABLE virtual void setGameAutoCheck(int id, bool val) = 0;
		Q_INVOKABLE virtual bool getGameAutoCheck(int id) = 0;
		Q_INVOKABLE virtual QString getGameName(int id) = 0;
		Q_INVOKABLE virtual QString  getConfigJsonUrl() = 0;
		Q_INVOKABLE virtual int getGameId() = 0;
		Q_INVOKABLE virtual QString getGamePresentationUrl(int id) = 0;
		Q_INVOKABLE virtual QString getPresentationFile(int id) = 0;
		Q_INVOKABLE virtual int getDefaultGameId() = 0;
		Q_INVOKABLE virtual QString getCurrentDirectory() = 0;
	};
}

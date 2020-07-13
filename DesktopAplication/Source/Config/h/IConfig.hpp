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

		virtual  Q_INVOKABLE bool installed(int id) = 0;
		virtual  Q_INVOKABLE QString gameInfoDir(int id) = 0;
		virtual  Q_INVOKABLE QUrl defaultInstallDir() = 0;
		virtual  Q_INVOKABLE void setDownloadSpeed(qint32 dp) = 0;
		virtual  Q_INVOKABLE qint32 getDownloadSpeed() = 0;
		virtual  Q_INVOKABLE void setGameAutoCheck(int id, bool val) = 0;
		virtual  Q_INVOKABLE bool getGameAutoCheck(int id) = 0;
		virtual  Q_INVOKABLE QString getGameName(int id) = 0;
		virtual  Q_INVOKABLE QString  getConfigJsonUrl() = 0;
		virtual  Q_INVOKABLE int getGameId() = 0;
		virtual  Q_INVOKABLE QString getGamePresentationUrl(int id) = 0;
		virtual  Q_INVOKABLE QString getPresentationFile(int id) = 0;
		virtual  Q_INVOKABLE int getDefaultGameId() = 0;
		virtual  Q_INVOKABLE QString getCurrentDirectory() = 0;
	};
}

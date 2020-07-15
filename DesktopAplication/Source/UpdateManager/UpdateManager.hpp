#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "Game.hpp"
#include "IConfig.hpp"
#include "IUpdateManager.hpp"
#include <memory>

namespace im {
	class UpdateInfo;
	class Downloader;
	class ArchieveInstaller;
	class Cleanup;
	class AppInfoParser;
	class FileListParser;
	class GameParser;

	class UpdateManager : public IUpdateManager {
		Q_OBJECT
	public:
		using files = std::vector<cf::AppPack>;

		virtual ~UpdateManager();
		UpdateManager();

		void update() {};
		std::string getName() { return TYPENAME(UpdateManager); }
		void init();

		// implementation IInstalationManager 
		void downloadStatus(qint64 progress, qint64 total, double speed);
		void installStatus(qint64 progress);
		UpdateInfo& getUpdateInfo() { return *updateInfo_; }

		void setTotal(qint64 tot);

		bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl, bool fullInstall);
		bool updateGame(cf::Game& game);


		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }

		void pause();
		void resume();
		void stop();

		void run();

	signals:
		void errorEmit(const QString& errorStr);
		void updateProgress(double prog);

		// AppUpdater
		void updateStatus(im::State needUpdate);
		void readGameInfo();
		void updateEnded(const QString& Version);

		// loadingBar
		void setTotalLb(double tot);
		void setActualLb(double act);
		void setSpeedLb(double sp);
		void setStateLb(im::State st);
		void setVisibleStateLb(im::VisibleState st);
		void setUninstallModeLb(bool un);

	private:
		void updateMainApp();
		void updateGame();
		void updateApp();
		void downloadUpdate();
		void installUpdate();
		void updateGameInfo();
		void updateGamePages();
		void reset();
		void setProgress();
		void sendDataToBar();
		void cleanUp();

	private:
		std::unique_ptr<UpdateInfo> updateInfo_;
		std::unique_ptr<Downloader> downloader_;
		std::unique_ptr<ArchieveInstaller> installer_;
		std::unique_ptr<Cleanup> cleanUpper_;
		std::unique_ptr<AppInfoParser> appInfoParser_;
		std::unique_ptr<FileListParser> fileListParser_;
		std::unique_ptr<GameParser> gameParser_;

		qint64 totalBytes_ = 0; // total Bytes to download unpack all files together
		qint64 downloadedBytes_ = 0; // Bytes downloaded
		qint64 unpackedBytes_ = 0; // Bytes Unpacked
		qint64 ProgressBytes_ = 0; // dwonloaded + unpacked

		//qml properties
		double progress_ = 0;
		double actual_ = 0;
		double total_ = 0;
		double speed_ = 0;
		int error_ = 0;
		QString errorStr_ = "";
	};
}

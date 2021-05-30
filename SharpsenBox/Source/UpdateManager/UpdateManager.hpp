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

namespace sb {
	class UpdateInfo;
	class Downloader;
	class ArchieveInstaller;
	class Cleanup;
	class AppInfoParser;
	class FileListParser;
	class GameParser;

	class UpdateManager final : public QThread, public IUpdateManager {
		Q_OBJECT
	public:
		using files = std::vector<AppPack>;

		virtual ~UpdateManager();
		UpdateManager();

		void Update(){};
		std::string GetName() { return TYPENAME(UpdateManager); }
		void Init();

		// implementation IInstalationManager 
		void downloadStatus(qint64 progress, qint64 total, double speed);
		void installStatus(qint64 progress);
		UpdateInfo& getUpdateInfo() { return *_UpdateInfo; }

		void setTotal(qint64 tot);

		bool installMainApp(QString version, QString appInfoUrl, QString gamesRepoUrl);
		bool updateMainApp(QString version, QString appInfoUrl, QString gamesRepoUrl);
		bool installGame(Game& game, const QString& gamePath, bool shortcut);
		bool updateGame(Game& game);


		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }

		void pause();
		void resume();

		void paused() final { pausedSignal(); };
		void resumed() final { resumedSignal(); }
		void stop() final;

	signals:
		void errorEmit(const QString& errorStr);
		void updateProgress(double prog);

		// AppUpdater
		void updateEnded(const QString& Version);

		// loadingBar
		void setTotalLb(double tot);
		void setActualLb(double act);
		void setSpeedLb(double sp);
		void setStateLb(int st);
		void setVisibleStateLb(int st);
		void setUninstallModeLb(bool un);

		void pausedSignal();
		void resumedSignal();

	private:
		// becouse connection signal -> sot doesnt work for direct enum, idk why
		void emitState(IUpdateManager::State st) { setStateLb(static_cast<int>(st)); }
		void emitVisibleState(IUpdateManager::VisibleState st) { setVisibleStateLb(static_cast<int>(st)); }
		void run() override;

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
		void cleanUp(IUpdateManager::State finalState, const QString& errorWhat);

	private:
		std::unique_ptr<UpdateInfo> _UpdateInfo;
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

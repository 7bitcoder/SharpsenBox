#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "Game.hpp"
#include "IConfig.hpp"
#include "LoadingBarStates.hpp"
#include "IInstalationManager.hpp"
#include <memory>

namespace upd {
	enum State : int {
		NONE = 0, DOWNLOADING, INSTALLING, ENDED, ERROR, NO_UPDATE_FOUND, UPDATING_GAME_PAGES
	};
}

namespace im {
	class UpdateInfo;
	class Downloader;
	class ArchieveInstaller;
	class Cleanup;
	class AppInfoParser;
	class FileListParser;
	class GameParser;

	class InstalationManager final : public IInstalationManager, public bc::Get<im::IInstalationManager> {
		Q_OBJECT
	public:
		using files = std::vector<cf::AppPack>;

		virtual ~InstalationManager();
		InstalationManager();

		// implementation IComponent
		void update() final {};
		std::string getName() final { return TYPENAME(InstalationManager); }
		void init() final;

		// implementation IInstalationManager 
		void downloadStatus(qint64 progress, qint64 total, double speed) final;
		void installStatus(qint64 progress) final;
		UpdateInfo& getUpdateInfo() final { return *updateInfo_; }

		void setTotal(qint64 tot);

		bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl, bool fullInstall) final;
		bool updateGame(cf::Game& game) final;


		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }

		void pause();
		void resume();
		void stop();

		void run() final;

	signals:
		void errorEmit(const QString& errorStr);
		void updateProgress(double prog);

		// AppUpdater
		void updateStatus(upd::State needUpdate);
		void readGameInfo();
		void updateEnded(const QString& finalVersion);

		// loadingBar
		void setTotalLb(double tot);
		void setActualLb(double act);
		void setSpeedLb(double sp);
		void setStateLb(lb::State st);
		void setVisibleStateLb(lb::VisibleState st);
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

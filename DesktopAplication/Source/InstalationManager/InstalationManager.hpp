#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "Downloader.hpp"
#include "ArchieveInstaller.hpp"
#include "IComponent.hpp"
#include "Config.hpp"
#include "Cleanup.hpp"
#include "AppInfoParser.hpp"
#include "FileListParser.hpp"
#include "UpdateInfo.hpp"
#include "GameParser.hpp"
#include "LoadingBar.hpp"
#include "IInstalationManager.hpp"

namespace im {
	class InstalationManager : public QThread, public bc::IComponent<InstalationManager>, public IInstalationManager {
		Q_OBJECT
	public:
		using files = std::vector<cf::AppPack>;

		virtual ~InstalationManager();
		InstalationManager();

		// implementation IComponent
		void update() override {};
		std::string getName() override { return TYPENAME(InstalationManager); }
		void init() override;

		// implementation IInstalationManager 
		void downloadStatus(qint64 progress, qint64 total, double speed) override;
		void installStatus(qint64 progress) override;
		UpdateInfo& getUpdateInfo() override { return updateInfo_; }

		void clearDownloadDir();
		void setTotal(qint64 tot);

		bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl, bool fullInstall);
		bool updateGame(cf::Game& game);
		void updateGamePages(files& files);


		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }

		void pause();
		void resume();
		void stop();

	private:
		void updateMainApp();
		void updateGame();
		void run() override;
		void updateApp();
		void downloadUpdate();
		void installUpdate();
		void updateGameInfo();
		void updateGamePages();
		void install(files files, qint64 tot, std::filesystem::path destination, cf::Game* game);
		void reset();
		void disconnectAll() {};
		void setProgress();
		void sendDataToBar();
		void cleanUp();

	public slots:


		void TotalSize(qint64 total);
		void errorCatched(int code);
		void downloadEnded(bool cancelled);
		void installEnded();
		void cleanUpEnded();

		void appInfoDownloaded();
		void downloadUpdateMetadata();
		void metadataDownloaded();
		void fileListParseEnded();
	signals:
		void errorEmit(const QString& errorStr);

		// AppUpdater
		void updateStatus(bool needUpdate);
		void readGameInfo();
		void updateEnded(const QString& finalVersion);

		// loadingBar
		void setTotalLb(double tot);
		void setActualLb(double act);
		void setProgressLb(double prog);
		void setSpeedLb(double sp);
		void setStateLb(lb::LoadingBar::State st);
		void setVisibleStateLb(lb::LoadingBar::VisibleState st);
		void setUninstallModeLb(bool un);
	private:
		lb::LoadingBar* LoadingBar_ = nullptr;

		UpdateInfo updateInfo_;
		Downloader downloader_;
		ArchieveInstaller installer_;
		Cleanup cleanUpper_;
		AppInfoParser appInfoParser_;
		FileListParser fileListParser_;
		GameParser gameParser_;

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

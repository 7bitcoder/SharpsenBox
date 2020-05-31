#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "FtpDownloader.hpp"
#include "IQmlObject.hpp"

namespace bb {
	class DownloadManager : public bc::IQmlObject {
		Q_OBJECT
	public:
		static DownloadManager& getObject() {
			static DownloadManager uc;
			return uc;
		}
		enum Stage : int {
			NONE = 0, DOWNLOAD, INSTALL
		};
		enum  DownloadState : int {
			CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET
		};
		enum  VisibleState : int {
			HIDDEN = 0, SHOWED, MINIMALIZED
		};



		// implementation IQmlObject
		void update() override;
		std::string getName() override;
		void init() override;

		//QML Propetries
		Q_PROPERTY(double speed READ getSpeed NOTIFY speedChanged);
		Q_PROPERTY(int downloadState READ getDownloadState NOTIFY downloadStateChanged);
		Q_PROPERTY(int visibleState READ getVisibleState NOTIFY visibleStateChanged);
		Q_PROPERTY(bool hideLock READ getHideLock NOTIFY hideLockChanged);
		Q_PROPERTY(double actual READ getActual NOTIFY actualChanged);
		Q_PROPERTY(double total READ getTotal NOTIFY totalChanged);
		Q_PROPERTY(int error READ getError  NOTIFY errorChanged);
		Q_PROPERTY(QString errorString READ getErrorString);

		//QMl invoklabes
		Q_INVOKABLE double getSpeed() const;
		Q_INVOKABLE int getDownloadState() const;
		Q_INVOKABLE int getVisibleState() const;
		Q_INVOKABLE bool getHideLock() const;
		Q_INVOKABLE double getActual() const;
		Q_INVOKABLE double getTotal() const;
		Q_INVOKABLE int getError() const;
		Q_INVOKABLE QString getErrorString() const;


		void downloadFile(std::string url, std::string fileName);
		void downloadLauchBoxJson(std::string url, std::string fileName);
		std::filesystem::path& getDownloadDir() { return downloadDir; }
	private:
		virtual ~DownloadManager();
		DownloadManager() {};
	public slots:
		void download(bool minimalLock);
		void status(qint64 progress, qint64 total, double speed);
		void TotalSize(qint64 total);
		void pauseD();
		void resumeD();
		void stopD();
		void errorCatched(int code);
		void termination();
	signals:
		void speedChanged();
		void downloadStateChanged();
		void visibleStateChanged();
		void hideLockChanged();
		void actualChanged();
		void totalChanged();
		void errorChanged();
		void notifyDownload();

		//signals to ftp thread
		void startDownloading();
		void resumeDownloading();
		void pauseDownloading();
		void stopDownloading();

	private:
		Stage stage_ = Stage::NONE;
		DownloadState downloadState_ = DownloadState::DOWNLOADING;
		VisibleState visibleState_ = VisibleState::SHOWED;
		FtpDownloader ftp_;
		std::filesystem::path downloadDir = "./Download";

		//qml properties
		bool hideLock_ = false;
		double progress_ = 0;
		double total_ = 0;
		double speed_ = 0;
		qint64 downloaded_ = 0;
		QThread ftpThread_;
		double actual_ = 0;
		int error_ = 0;
		QString errorStr_ = "";
	};
}
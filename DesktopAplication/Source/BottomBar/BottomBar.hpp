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
	class BottomBar : public bc::IQmlObject {
		Q_OBJECT
	public:
		enum  DownloadState : int {
			checking = 0, downloading, installing, pause, error, stopped, completed
		};
		enum  VisibleState : int {
			hidden = 0, showed, minimalized
		};

		virtual ~BottomBar();
		BottomBar() :ftp_(*this) {}

		// implementation IQmlObject
		void update() override;
		std::string getName() override;
		void init() override {}

		//QML Propetries
		Q_PROPERTY(double progress READ getProgress NOTIFY progresChanged);
		Q_PROPERTY(int downloadState READ getDownloadState NOTIFY downloadStateChanged);
		Q_PROPERTY(int visibleState READ getVisibleState NOTIFY visibleStateChanged);
		Q_PROPERTY(bool hideLock READ getHideLock NOTIFY hideLockChanged);
		Q_PROPERTY(double actual READ getActual NOTIFY actualChanged);
		Q_PROPERTY(double total READ getTotal NOTIFY totalChanged);

		//QMl invoklabes
		Q_INVOKABLE double getProgress() const;
		Q_INVOKABLE int getDownloadState() const;
		Q_INVOKABLE int getVisibleState() const;
		Q_INVOKABLE bool getHideLock() const;
		Q_INVOKABLE double getActual() const;
		Q_INVOKABLE double getTotal() const;


	public slots:
		void download();
		void status(qint64 progress, qint64 total);
		void TotalSize(qint64 total);
		void pauseD();
		void resumeD();
		void stopD();

		void termination();
	signals:
		void progresChanged();
		void downloadStateChanged();
		void visibleStateChanged();
		void hideLockChanged();
		void actualChanged();
		void totalChanged();

		//signals to ftp thread
		void startDownloading();
		void resumeDownloading();
		void pauseDownloading();
		void stopDownloading();

	private:
		DownloadState downloadState_ = DownloadState::downloading;
		VisibleState visibleState_ = VisibleState::showed;
		FtpDownloader ftp_;
		bool hideLock_ = false;
		double progress_ = 0;
		double total_ = 0;
		qint64 downloaded_ = 0;
		QThread ftpThread_;
		double actual_ = 0;
	};
}
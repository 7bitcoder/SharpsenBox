#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "FtpDownloader.hpp"
#include "IQmlObject.hpp"
namespace lb {
	class LoadingBar;
}
namespace bb {
	class InstalationManager : public bc::IQmlObject {
		Q_OBJECT
	public:
		static InstalationManager& getObject() {
			static InstalationManager uc;
			return uc;
		}
		enum Stage : int {
			NONE = 0, DOWNLOAD, INSTALL
		};
		enum  State : int {
			CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET
		};
		enum  VisibleState : int {
			HIDDEN = 0, SHOWED, MINIMALIZED
		};


		//!!!!! disconnect(receiver, SLOT(slot()));@ !! disconnect

		// implementation IQmlObject
		void update() override;
		std::string getName() override;
		void init() override;

		void downloadFile(std::filesystem::path fileName);
	private:
		virtual ~InstalationManager();
		InstalationManager() {};
	public slots:
		void download();
		void status(qint64 progress, qint64 total, double speed);
		void TotalSize(qint64 total);
		void pauseD();
		void resumeD();
		void stopD();
		void errorCatched(int code);
		void termination();
	signals:
		void notifyDownload();


	private:
		lb::LoadingBar* LoadingBar_ = nullptr;
		Stage stage_ = Stage::NONE;
		State state_ = State::DOWNLOADING;
		VisibleState visibleState_ = VisibleState::HIDDEN;
		FtpDownloader ftp_;
		std::filesystem::path downloadDir_;

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
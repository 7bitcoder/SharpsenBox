#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "FtpDownloader.hpp"
#include "ArchieveInstaller.hpp"
#include "IQmlObject.hpp"
#include "Config.hpp"

namespace lb {
	class LoadingBar;
}
namespace bb {
	class InstalationManager : public bc::IQmlObject {
		Q_OBJECT
	public:
		using files = std::vector<std::filesystem::path>;
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
		void update() override {};
		std::string getName() override;
		void init() override;
		//
		void clearDownloadDir();
		void setTotal(qint64 tot);
		void downloadFile(std::filesystem::path fileName, qint64 tot); //just download
		void installFile(std::filesystem::path fileName, qint64 tot, std::filesystem::path dir = "../"); //download + install
		
		void installGame(const cf::Game& game); //download + install

		void downloadFiles(files files, qint64 tot); //just download
		void installFiles(files files, qint64 tot, std::filesystem::path dir = "../"); //download + install
		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }
	private:
		virtual ~InstalationManager();
		InstalationManager() {};
		void download(); //just download
		void install(); //download + install
		void reset();
		void disconnectAll() {};
		void setProgress();
		void sendDataToBar();
	public slots:
		void downloadStatus(qint64 progress, qint64 total, double speed);
		void installStatus(qint64 progress);
		void TotalSize(qint64 total);
		void pauseD();
		void resumeD();
		void stopD();
		void errorCatched(int code);
		void ftpEnded();
		void archieveEnded();
	signals:
		void errorEmit();
		void downloadEnded();
		void installEnded();
		void clearFilesEnded();
	private:
		bool onlyDownload;
		files files_;
		lb::LoadingBar* LoadingBar_ = nullptr;
		Stage stage_ = Stage::NONE;
		State state_ = State::CHECKING;
		VisibleState visibleState_ = VisibleState::HIDDEN;
		FtpDownloader ftp_;
		ArchieveInstaller installer_;
		std::filesystem::path downloadDir_;
		std::filesystem::path installDir_;

		qint64 totalBytes_ = 0; //total Bytes to download unpack all files together
		qint64 downloadedBytes_ = 0; //Bytes downloaded
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
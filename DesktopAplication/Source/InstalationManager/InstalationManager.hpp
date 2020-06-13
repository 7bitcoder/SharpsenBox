#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "Downloader.hpp"
#include "ArchieveInstaller.hpp"
#include "IQmlObject.hpp"
#include "Config.hpp"
#include "Cleanup.hpp"

namespace lb {
	class LoadingBar;
}
namespace bb {
	class InstalationManager : public bc::IQmlObject {
		Q_OBJECT
	public:
		using files = std::vector<std::pair<std::filesystem::path, std::string>>;
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

		// implementation IQmlObject
		void update() override {};
		std::string getName() override;
		void init() override;

		// interface
		void clearDownloadDir();
		void setTotal(qint64 tot);

		void downloadFile(std::filesystem::path url, std::string fileName, qint64 tot); //just download
		void installFile(std::filesystem::path url, std::string fileName, qint64 tot, std::filesystem::path dir = "../", cf::Game* game = nullptr); //download + install
		void downloadFiles(files files, qint64 tot); //just download
		void installFiles(files files, qint64 tot, std::filesystem::path dir = "../", cf::Game* game = nullptr); //download + install

		void installGame(cf::Game& game); //download + install

		double getProgress() { return progress_; }
		double getTotal() { return total_; }
		double getSpeed() { return speed_; }
		double getActual() { return actual_; }
		QString getError() { return errorStr_; }

		void pause();
		void resume();
		void stop();
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
		void errorCatched(int code);
		void ftpEnded(bool cancelled);
		void archieveEnded();
		void cleanUpEnded();
	signals:
		void errorEmit();
		void downloadEnded();
		void installEnded();
		void clearFilesEnded();
	private:
		bool onlyDownload;
		bool cancel_;
		files files_;

		cf::Game* actualGame_ = nullptr;
		lb::LoadingBar* LoadingBar_ = nullptr;
		
		Stage stage_ = Stage::NONE;
		State state_ = State::CHECKING;
		VisibleState visibleState_ = VisibleState::HIDDEN;
		
		Downloader downloader_;
		ArchieveInstaller installer_;
		cu::Cleanup cleanUpper_;

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

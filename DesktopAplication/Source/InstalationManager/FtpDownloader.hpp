#pragma once 

#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "IQmlObject.hpp"
#include <filesystem>

namespace bb {
	class FtpDownloader : public  QThread {
		Q_OBJECT
	public:
		using files = std::vector < std::filesystem::path>;
		FtpDownloader();
		~FtpDownloader() {}
		void setOutputFile(std::string file) { outfile_ = file; }
		void setUrl(std::string url) { url_ = url; }
		qint64 getTotal() { return total_; }
		qint64 getProgress() { return now_; }
		void setFilestoDownload(files file) { files_ = file; }

		std::atomic_flag pause;
		std::atomic_flag resume;
		std::atomic_flag stop;
		void run() override;
	signals:
		void statusSignal(qint64 progress, qint64 total, double speed);
		void ended();
		void error(int);
	public slots:
	private:
		void emitStatus();
		static size_t my_fwrite(void* buffer, size_t size, size_t nmemb, void* userdata);
		static int progress_callback(void* clientp, long long dltotal, long long dlnow, long long ultotal, long long ulnow);
		int checkState();
	private:
		std::filesystem::path outfile_ = "curl.tar.gz";
		std::string url_;
		qint64 total_ = 0;
		qint64 now_ = 0;
		double speed_;
		FILE* stream_ = nullptr;
		void* curl;
		int res;
		bool cancelled = false;
		files files_;
	};
}
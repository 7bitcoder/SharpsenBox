#pragma once 

#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include <curl/curl.h>
#include "IQmlObject.hpp"


namespace bb {
	class FtpDownloader : public  QThread {
		Q_OBJECT
	public:
		FtpDownloader() = default;
		FtpDownloader(bc::IQmlObject& update) : update_(update) {}
		void setOutputFile(std::string file) { outfile_ = file; }
		void setUrl(std::string url) { url_ = url; }
		qint64 getTotal() { return total_; }
		qint64 getProgress() { return now_; }
		void setFilestoDownload(std::vector<std::string> files) { files_ = files; }
		~FtpDownloader() {}

		std::atomic_flag pause;
		std::atomic_flag resume;
		std::atomic_flag stop;
		void run() override;
	signals:
		void statusSignal(qint64 progress, qint64 total, double speed);
		void ended();
	public slots:
	private:
		void emitStatus();
		static size_t my_fwrite(void* buffer, size_t size, size_t nmemb, void* userdata);
		static int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
		int checkState();
	private:
		std::string outfile_ = "curl.tar.gz";
		std::string url_;
		qint64 total_ = 0;
		qint64 now_ = 0;
		double speed_;
		FILE* stream_ = nullptr;
		bc::IQmlObject& update_;
		CURL* curl;
		CURLcode res;
		bool cancelled = false;
		std::vector<std::string> files_ = { "ftp://speedtest.tele2.net/10GB.zip" };
	};
}
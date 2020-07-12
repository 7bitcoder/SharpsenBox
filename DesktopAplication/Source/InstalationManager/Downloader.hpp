#pragma once 

#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "ImElement.hpp"
#include <filesystem>

namespace cf {
	struct AppPack;
}

namespace im {
	class Downloader : public ImElement {
		Q_OBJECT
	public:
		using files = std::vector < cf::AppPack >;
		Downloader();
		~Downloader() {}

		// ImElement implementation
		bool run() override;
		void reset() override;
	private:
		static size_t writeToFile(void* buffer, size_t size, size_t nmemb, void* userdata);
		static int progressCallback(void* clientp, long long dltotal, long long dlnow, long long ultotal, long long ulnow);
		void closeFile();

		int checkState();
		void emitStatus();
		void checkSpeed();

		void setErrorStr(int code);
	private:
		std::filesystem::path outfile_ = "";
		qint64 total_ = 0;
		qint64 now_ = 0;
		qint64 lastDownload_ = 0;
		qint32 downloadSpeed_ = 0;
		double speed_;

		FILE* stream_ = nullptr;
		void* curl;
		int res;

		bool cancelled = false;
	};
}

#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>
#include "IRunnable.hpp"

namespace sb {
	struct AppPack;
}

namespace sb {
	class AppInfoParser :public IRunnable {
	public:
		using files = std::vector< AppPack >;

		bool needUpdate() { return needUpdate_; };

		files& getFiles() { return pathFiles_; }
		QString& getVersionToUpdate() { return versionToUpdate_; }

		// IRunnable implementation
		void run() override;
		void reset() override;
	private:
		void getPathUrls(const QJsonObject& pathList);

		QString versionToUpdate_;
		std::filesystem::path parseInfoFileName = "AppInfo.json";

		bool needUpdate_ = false;
		// fileName -> <fileUrl, size>
		// first element is filelist
		files pathFiles_;
	};
}

#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>
#include "ImElement.hpp"

namespace cf {
	struct AppPack;
}

namespace im {
	class AppInfoParser :public ImElement {
	public:
		using files = std::vector< cf::AppPack >;

		bool needUpdate() { return needUpdate_; };

		files& getFiles() { return pathFiles_; }
		QString& getFileListUrl() { return fileListUrl_; }

		// ImElement implementation
		bool run() override;
		void reset() override;
	private:
		void getPathUrls(QJsonObject& pathList);

		std::filesystem::path parseInfoFileName = "AppInfo.json";
		QString fileListUrl_;

		bool needUpdate_ = false;
		// fileName -> <fileUrl, size>
		// first element is filelist
		files pathFiles_;
	};
}

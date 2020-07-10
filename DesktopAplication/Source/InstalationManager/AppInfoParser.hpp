#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>

namespace cf {
	struct AppPack;
}

namespace bb {
	class AppInfoParser :public QThread {
		Q_OBJECT
	public:
		using files = std::vector< cf::AppPack >;
		void setVerToCheck(QString ver) { actualVersion_ = ver; }
		QString getVertoUpdate() { return versionToUpdate_;  }
		void parse( bool fullInstall);
		bool needUpdate() { return needUpdate_; };
		void reset();
		files& getFiles() { return pathFiles_; }
		void run() override;
		QString& getFileListUrl() {
			return fileListUrl_;
		}
	signals:
		void parseEnded();
	private:
		void getPathUrls(QJsonObject& pathList);
		QString actualVersion_;
		QString versionToUpdate_;
		std::filesystem::path parseInfoFileName = "AppInfo.json";
		QString fileListUrl_;
		bool needUpdate_ = false;
		bool fullInstall_ = false;
		// fileName -> <fileUrl, size>
		// first element is filelist
		files pathFiles_;
	};
}

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
		// versions
		void setActualVer(QString ver) { actualVersion_ = ver; }
		QString getUpdateVer() { return versionToUpdate_;  }

		void parse( bool fullInstall);
		bool needUpdate() { return needUpdate_; };
		
		files& getFiles() { return pathFiles_; }
		QString& getFileListUrl() {	return fileListUrl_; }

		void reset();
	signals:
		void parseEnded();
	private:
		void run() override;
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

#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>

namespace bb {
	class AppInfoParser :public QThread {
		Q_OBJECT
	public:
		using files = std::vector<std::pair<std::filesystem::path, std::string>>;
		void setVerToCheck(QString ver) { actualVersion_ = ver; }
		QString getVer() { return actualVersion_;  }
		void parse();
		bool needUpdate() { return needUpdate_; };
		void reset();

		void run() override;
		QString& getFileListUrl() {
			return fileListUrl_;
		}
	signals:
		void parseEnded();
	private:
		QString actualVersion_;
		std::filesystem::path parseInfoFileName = "AppInfo.json";
		QString fileListUrl_;
		bool needUpdate_ = false;
		// fileName -> <fileUrl, size>
		std::unordered_map < QString, std::pair < QString, qint64>> pathFiles_;
	};
}

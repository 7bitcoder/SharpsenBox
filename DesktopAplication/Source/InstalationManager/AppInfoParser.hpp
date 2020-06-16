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
		files getNeededFiles() { return files_; }
		qint64 getBytesToDownload() { return totalBytesTo_; }
		void reset();

		void run() override;
		void getAllFiles(QJsonDocument& doc);
		void getDeltaFiles(QJsonDocument& doc);
	signals:
		void parseEnded();
	private:
		QString actualVersion_;
		files files_;
		qint64 totalBytesTo_ = 0;
		std::filesystem::path parseInfoFileName = "AppInfo.json";
		bool needUpdate_ = false;
		// fileName -> <fileUrl, size>
		std::unordered_map < QString, std::pair < QString, qint64>> allFiles_;
	};
}

#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_set>
#include <QJsonDocument>

namespace bb {
	class FileListParser :public QThread {
		Q_OBJECT
	public:
		using files = std::vector<std::pair<std::filesystem::path, std::string>>;
		void setActualVersion(QString ver) { actualVersion_ = ver; }
		void setVersionToUpdate(QString ver) { toUpdateVersion_ = ver; }
		void setPathFiles(files& files) { pathFiles_ = files; }
		QString getVer() { return actualVersion_;  }
		void parse(bool fullInstall);
		bool needUpdate() { return needUpdate_; };
		files getNeededFiles() { return files_; }
		std::unordered_set<QString>& getFilesToDelete() { return toRemove_; };
		qint64 getBytesToDownload() { return totalBytesTo_; }
		void reset();

		void run() override;
		void getAllFiles(QJsonDocument& doc);
		void getDeltaFiles(QJsonDocument& doc);
	signals:
		void parseEnded();
	private:
		void readPackets();
		void readAllPackets();
		QString actualVersion_;
		QString toUpdateVersion_;
		files files_;
		files pathFiles_;
		qint64 totalBytesTo_ = 0;
		std::filesystem::path parseInfoFileName = "FileList.json";
		bool needUpdate_ = false;
		bool fullInstall_ = false;
		QJsonDocument fileList_;

		std::unordered_set<QString> toDownload_;
		std::unordered_set<QString> toRemove_;
	};
}

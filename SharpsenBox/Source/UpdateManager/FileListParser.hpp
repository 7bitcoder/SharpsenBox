#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_set>
#include "IRunnable.hpp"
#include <QJsonDocument>

namespace sb {
	struct AppPack;
}

namespace sb {
	class FileListParser :public IRunnable {
	public:
		using files = std::vector<AppPack>;

		// interface
		files& getNeededFiles() { return files_; }
		std::unordered_set<QString>& getFilesToDelete() { return toRemove_; };
		qint64 getBytesToDownload() { return totalBytesTo_; }

		// ImElement implementation
		void Run() override;
		void Reset() override;

	private:
		void readPackets();
		void readAllPackets();

		files files_;
		files pathFiles_;
		qint64 totalBytesTo_ = 0;

		QString parseInfoFileName = "FileList.json";

		QJsonDocument fileList_;

		std::unordered_set<QString> toDownload_;
		std::unordered_set<QString> toRemove_;
	};
}

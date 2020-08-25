#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_set>
#include "IRunnable.hpp"
#include <QJsonDocument>

namespace cf {
	struct AppPack;
}

namespace im {
	class FileListParser :public IRunnable {
	public:
		using files = std::vector<cf::AppPack>;

		// interface
		files& getNeededFiles() { return files_; }
		std::unordered_set<QString>& getFilesToDelete() { return toRemove_; };
		qint64 getBytesToDownload() { return totalBytesTo_; }

		// ImElement implementation
		void run() override;
		void reset() override;

	private:
		void readPackets();
		void readAllPackets();

		files files_;
		files pathFiles_;
		qint64 totalBytesTo_ = 0;

		std::filesystem::path parseInfoFileName = "FileList.json";

		QJsonDocument fileList_;

		std::unordered_set<QString> toDownload_;
		std::unordered_set<QString> toRemove_;
	};
}

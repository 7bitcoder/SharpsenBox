#pragma once
#include <iostream>
#include <QObject>
#include <QThread>

namespace bb {
	class AppInfoParser :public QThread {
	public:
		using files = std::vector<std::pair<std::filesystem::path, std::string>>;
		void setVerToCheck(QString ver) { actualVersion_ = ver; }
		void parse();
		bool needUpdate();
		files getNeededFiles() {
			return files_;
		}
		qint64 getBytesToDownload() {}
	private:
		QString actualVersion_;
		files files_;
	};
}

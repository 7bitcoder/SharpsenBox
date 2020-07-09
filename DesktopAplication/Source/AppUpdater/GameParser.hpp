#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>

namespace upd {
	class GameParser :public QThread {
		Q_OBJECT
	public:
		using files = std::vector<std::pair<std::filesystem::path, std::string>>;
		void parse( );

		files& getFiles() { return files_; }
		bool needUpdate() { return files_.size(); }
		void updateGamesInfo();
		void run() override;
		void reset();
	signals:
		void parseEnded();
	private:
		// fileName -> <fileUrl, size>
		// first element is filelist
		std::vector < std::pair<std::filesystem::path, std::string >> files_;
		std::vector < std::pair<int, QString>> toUpdate_;
	};
}

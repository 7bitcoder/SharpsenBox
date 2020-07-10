#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>
#include "Config.hpp"
namespace upd {
	class GameParser :public QThread {
		Q_OBJECT
	public:
		using files = std::vector<cf::AppPack>;
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
		files files_;
		std::vector < std::pair<int, QString>> toUpdate_;
	};
}

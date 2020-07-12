#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>
#include "Config.hpp"
#include "IRunnable.hpp"

namespace im {
	class GameParser :public IRunnable {
	public:
		using files = std::vector<cf::AppPack>;

		files& getFiles() { return files_; }
		bool needUpdate() { return files_.size(); }

		void updateGamesInfo();

		// ImElement implementation
		bool run() override;
		void reset() override;
	private:
		// fileName -> <fileUrl, size>
		// first element is filelist
		files files_;
		std::vector < std::pair<int, QString>> toUpdate_;
	};
}

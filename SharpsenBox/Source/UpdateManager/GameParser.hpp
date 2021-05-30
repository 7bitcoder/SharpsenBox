#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <unordered_map>
#include "Game.hpp"
#include "IRunnable.hpp"

namespace sb {
	class GameParser :public IRunnable {
	public:
		using files = std::vector<AppPack>;

		files& getFiles() { return files_; }
		bool needUpdate() { return files_.size(); }

		void updateGamesInfo();

		// ImElement implementation
		void Run() override;
		void Reset() override;
	private:
		// fileName -> <fileUrl, size>
		// first element is filelist
		files files_;
		std::vector < std::pair<int, QString>> toUpdate_;
	};
}

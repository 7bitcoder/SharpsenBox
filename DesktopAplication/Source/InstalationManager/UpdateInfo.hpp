#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <filesystem>
#include <atomic>
#include <unordered_map>
#include "Config.hpp"
#include "IRunnable.hpp"

namespace cf {
	struct AppPack;
}

namespace im {
	class UpdateInfo : public IRunnable {
	public:
		//class holind infomration about updating process
		using files = std::vector< cf::AppPack >;
		enum UpdateMode : int { GAME = 0, LAUNCHBOX };

		UpdateInfo();
		~UpdateInfo() {}

		//getters setters
		UpdateMode getUpdateMode() { return updateMode_; }
		void setUpdateMode(UpdateMode mode) { updateMode_ = mode; }

		files& getFiles() { return files_; }
		void setFiles(const files& files) { files_ = files; }

		bool getFullInstall() { return fullInstall_; }
		void setFullInstall(const bool fullInstall) { fullInstall_ = fullInstall; }

		QString& getActualVersion() { return actualVersion_; }
		void setActualVersion(const QString& ver) { actualVersion_ = ver; }

		QString& getUpdateVersion() { return toUpdateVersion_; }
		void setUpdateVersion(const QString& ver) { toUpdateVersion_ = ver; }

		cf::Game& getActualGame() { if (!actualGame_) throw std::exception("No Game is setUp"); return *actualGame_; }
		void setActualGame(cf::Game& game) { actualGame_ = &game; }
		void setActualGame(cf::Game* game) { actualGame_ = game; }

		std::filesystem::path& getDownloadDir() { return downloadDir_; }
		void setDownloadDir(const std::filesystem::path& dir) { downloadDir_ = dir; }

		std::filesystem::path& getInstallDir() { return installDir_; }
		void setInstallDir(const std::filesystem::path& dir) { installDir_ = dir; }

		bool isGameUppdating() { return getUpdateMode() == UpdateMode::GAME; }
		bool isLBUpdating() { return getUpdateMode() == UpdateMode::LAUNCHBOX; }

		void reset() override;
		bool run() override { return true; }

		//controlls 
		std::atomic_flag pause;
		std::atomic_flag resume;
		std::atomic_flag stop;
	private:
		UpdateMode updateMode_;
		bool cancel_;
		bool fullInstall_;

		files files_;
		QString actualVersion_;
		QString toUpdateVersion_;

		cf::Game* actualGame_ = nullptr;

		std::filesystem::path downloadDir_;
		std::filesystem::path installDir_;
	};
}

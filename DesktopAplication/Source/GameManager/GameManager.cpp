#include <iostream>
#include <string>
#include "GameManager.hpp"
#include "Config.hpp"
#include"InstalationManager.hpp"
#include "GameUninstaller.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"

namespace gm {

	std::string GameManager::getName() {
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : uninstaller_(new GameUninstaller) {};


	Q_INVOKABLE void GameManager::installGame(int id, QString path, bool shortcut) {
		if (!lock_) {
			lock();
			auto& game = cf::Config::getObject().getGame(id);
			std::string gg = path.toStdString();
#ifdef _WIN32
			if (gg.starts_with("/"))
				gg = gg.substr(1);
#endif 
			std::filesystem::path p = gg;
			p /= game.name.toStdString();
			game.gameDir = p.generic_string().c_str();
			game.shortcut = shortcut;
			bb::InstalationManager::getObject().updateGame(game);
		}
	}
	void GameManager::init() {
		connect(uninstaller_, &GameUninstaller::uninstalationComplete, this, &GameManager::uninstallation);
	};

	Q_INVOKABLE void GameManager::unistallRequest(int id) {
		Gameid_ = id;
		auto& dialog = dl::Dialog::getObject();
		connect(&dialog, &dl::Dialog::dialogTriggered, this, &GameManager::uninstall);
		dialog.setInfo(QString("Are you sure you want to remove ") + cf::Config::getObject().getGameName(id));
		dialog.show();
	}

	void GameManager::uninstall(bool dialogValue) {
		if (dialogValue) {
			lock();
			auto& lb = lb::LoadingBar::getObject();
			lb.reset();
			lb.setUninstallMode(true);
			lb.setState(lb::LoadingBar::State::CHECKING);
			lb.setVisibleState(lb::LoadingBar::VisibleState::SHOWED);
			uninstaller_->setId(Gameid_);
			uninstaller_->start();
		} else {} //nothing
		auto& dialog = dl::Dialog::getObject();
		disconnect(&dialog, &dl::Dialog::dialogTriggered, this, &GameManager::uninstall);
	}

	void GameManager::uninstallation(int id) {
		cf::Config::getObject().getGame(id).installed = false;
		unLock();
		auto& lb = lb::LoadingBar::getObject();
		lb.reset();
		lb.setUninstallMode(false);
		lb.setState(lb::LoadingBar::State::COMPLEET);
		lb.setVisibleState(lb::LoadingBar::VisibleState::HIDDEN);
	}
}

#include <iostream>
#include <string>
#include "GameManager.hpp"
#include "AppBackend.hpp"
#include "IConfig.hpp"
#include "Game.hpp"
#include "IInstalationManager.hpp"
#include "GameUninstaller.hpp"
#include "ILoadingBar.hpp"
#include "IDialog.hpp"

namespace gm {

	std::string GameManager::getName() {
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : uninstaller_(new GameUninstaller) {};


	Q_INVOKABLE void GameManager::installGame(int id, QString path, bool shortcut) {
		if (!lock_) {
			lock();
			auto& game = bc::Backend::getBackend().getConfig().getGame(id);
			std::string gg = path.toStdString();
#ifdef _WIN32
			if (gg.starts_with("/"))
				gg = gg.substr(1);
#endif 
			std::filesystem::path p = gg;
			p /= game.name.toStdString();
			game.gameDir = p.generic_string().c_str();
			game.shortcut = shortcut;
			bc::Backend::getBackend().getInstalationManager().updateGame(game);
		}
	}
	void GameManager::init() {
		connect(uninstaller_, &GameUninstaller::uninstalationComplete, this, &GameManager::uninstallation);
	};

	Q_INVOKABLE void GameManager::unistallRequest(int id) {
		Gameid_ = id;
		auto& dialog = bc::Backend::getBackend().getDialog();
		dialog.setInfo(QString("Are you sure you want to remove ") + bc::Backend::getBackend().getConfig().getGameName(id));
		dialog.show();
	}

	void GameManager::uninstall(bool dialogValue) {
		if (dialogValue) {
			lock();
			auto& lb = bc::Backend::getBackend().getLoadingBar();
			lb.reset();
			//lb.setUninstallMode(true);
			//lb.setState(lb::State::CHECKING);
			//lb.setVisibleState(lb::VisibleState::SHOWED);
			uninstaller_->setId(Gameid_);
			uninstaller_->start();
		} else {} //nothing
		auto& dialog = bc::Backend::getBackend().getDialog();
	}

	void GameManager::uninstallation(int id) {
		bc::Backend::getBackend().getConfig().getGame(id).installed = false;
		unLock();
		auto& lb = bc::Backend::getBackend().getLoadingBar();
		lb.reset();
		//lb.setUninstallMode(false);
		//lb.setState(lb::State::COMPLEET);
		//lb.setVisibleState(lb::VisibleState::HIDDEN);
	}

	Q_INVOKABLE void GameManager::checkAutoUpdate(int id) {
		auto& game = bc::Backend::getBackend().getConfig().getGame(id);
		if (game.autoCheck && game.installed) {
			update(id);
		}
	}

	Q_INVOKABLE void GameManager::update(int id) {
		auto& game = bc::Backend::getBackend().getConfig().getGame(id);
		if (game.updateChecked)
			return; //checked
		bc::Backend::getBackend().getInstalationManager().updateGame(game);
	}

	Q_INVOKABLE void GameManager::runGame(int id) {
		auto& game = bc::Backend::getBackend().getConfig().getGame(id);
		std::filesystem::path path = game.gameDir.toUtf8().constData();
		path /= game.execPath.toUtf8().constData();
		std::string cd = "cd \"";
		cd += path.parent_path().generic_string() + "\"";
		std::string execute = path.filename().generic_string();
		std::string total = cd + " && " + execute;
		system(total.c_str());
	}
}

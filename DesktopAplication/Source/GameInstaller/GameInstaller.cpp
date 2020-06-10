#include <iostream>
#include <string>
#include "GameInstaller.hpp"
#include "Config.hpp"
#include"InstalationManager.hpp"
#include "GameUninstaller.hpp"

namespace gi {

	std::string GameInstaller::getName() {
		return TYPENAME(GameInstaller);
	}
	GameInstaller::GameInstaller() : uninstaller_(new GameUninstaller) {};
	Q_INVOKABLE void GameInstaller::installGame(int id, QString path, bool shortcut) {
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
			bb::InstalationManager::getObject().installGame(game);
		}
	}
	void GameInstaller::init() {
		connect(uninstaller_, &GameUninstaller::uninstalationComplete, this, &GameInstaller::uninstallation);
	};
	Q_INVOKABLE void GameInstaller::unistall(int id) {
		gi::GameInstaller::getObject().lock();
		uninstaller_->setId(id);
		uninstaller_->start();
	}

	void GameInstaller::uninstallation(int id) {
		cf::Config::getObject().getGame(id).installed = false;
		gi::GameInstaller::getObject().unLock();
	}
}

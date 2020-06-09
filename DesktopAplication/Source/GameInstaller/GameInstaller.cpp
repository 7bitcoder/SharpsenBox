#include <iostream>
#include <string>
#include "GameInstaller.hpp"
#include "Config.hpp"
#include"InstalationManager.hpp"

namespace gi {

	std::string GameInstaller::getName() {
		return TYPENAME(GameInstaller);
	}

	Q_INVOKABLE void GameInstaller::installGame(int id, QString path, bool shortcut) {
		if (!lock_) {
			lock();
			auto& game = cf::Config::getObject().getGame(id);
			std::string gg = path.toStdString();
#ifdef _WIN32
			if (gg.starts_with("/"))
				gg = gg.substr(1);
#endif 
			game.gameDir = gg.c_str();
			game.shortcut = shortcut;
			bb::InstalationManager::getObject().installGame(game);
		}
	}
}
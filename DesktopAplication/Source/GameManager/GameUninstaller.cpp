#include "GameUninstaller.hpp"
#include "IConfig.hpp"
#include "ObjectRepo.hpp"
#include "Game.hpp"

namespace gm {
	void GameUninstaller::setId(int id) {
		id_ = id;
		auto& game = bc::ObjectsRepository::getRepo().getConfig().getGame(id_);
		gameDir_ = game.gameDir.toStdString();
		shortcut_ = game.shortcut;
		if (shortcut_)
			shortcutPath_ = game.shortcutPath.toStdString();
		start();
	}
	void GameUninstaller::run() {
		try {
			if (std::filesystem::exists(gameDir_))
				std::filesystem::remove_all(gameDir_);
			if (shortcut_ && std::filesystem::exists(shortcutPath_))
				std::filesystem::remove(shortcutPath_);
		} catch (...) {

		}
		uninstalationComplete(id_);
	}

}

#include "GameUninstaller.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "Game.hpp"

namespace sb {
	void GameUninstaller::setId(int id) {
		id_ = id;
		auto& game = Component<IConfig>::Get().GetGame(id_);
		gameDir_ = game.GameDir.toStdString();
		shortcut_ = game.HasShortcut;
		if (shortcut_)
			shortcutPath_ = game.ShortcutPath.toStdString();
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

#include "GameUninstaller.hpp"
#include "Config.hpp"

namespace gi {
	void GameUninstaller::setId(int id) {
		id_ = id;
		gameDir_ = cf::Config::getObject().getGame(id_).gameDir.toStdString();
		start();
	}
	void GameUninstaller::run() {
		try {
			if (std::filesystem::exists(gameDir_))
				std::filesystem::remove_all(gameDir_);
		} catch (...) {

		}
		uninstalationComplete(id_);
	}

}

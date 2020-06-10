#include <iostream>
#include <string>
#include "Config.hpp"
#include "GameExecutor.hpp"

namespace ge {

	std::string GameExecutor::getName() {
		return TYPENAME(GameExecutor);
	}

	Q_INVOKABLE void GameExecutor::execute(int id) {
		auto& game = cf::Config::getObject().getGame(id);
		std::filesystem::path path = game.gameDir.toUtf8().constData();
		path /= game.execDir.toUtf8().constData();
		auto& str = path.generic_string();
		system(path.generic_string().c_str());
	}

	Q_INVOKABLE void GameExecutor::uninstall(int id) {
		auto& game = cf::Config::getObject().getGame(id);
		std::filesystem::path path = game.gameDir.toUtf8().constData();
		path /= game.execDir.toUtf8().constData();
		auto& str = path.generic_string();
		system(path.generic_string().c_str());
	}
}

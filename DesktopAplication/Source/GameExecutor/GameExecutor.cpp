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
		std::string cd = "cd \"";
		cd += path.parent_path().generic_string() + "\"";
		std::string execute = path.filename().generic_string();
		std::string total = cd + " && " + execute;
		system(total.c_str());
	}
}

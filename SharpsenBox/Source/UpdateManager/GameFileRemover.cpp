#include "GameFileRemover.hpp"
#include "IConfig.hpp"
#include "Game.hpp"

namespace sb {
	void GameFileRemover::run() {
		try {
			std::filesystem::path gamePath = game_->gameDir.toStdString();
			for (auto& path : *toRemove_) {
				auto filePath = gamePath / path.toStdString();
				if (std::filesystem::exists(filePath))
					std::filesystem::remove(filePath);
			}
		} catch (...) {
			error("Error ocured while removing Game files");
		}
	}

}

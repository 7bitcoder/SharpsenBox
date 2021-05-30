#include "GameFileRemover.hpp"
#include "IConfig.hpp"
#include "Game.hpp"

namespace sb {
	void GameFileRemover::Run() {
		try {
			std::filesystem::path gamePath = game_->GameDir.toStdString();
			for (auto& path : *toRemove_) {
				auto filePath = gamePath / path.toStdString();
				if (std::filesystem::exists(filePath))
					std::filesystem::remove(filePath);
			}
		} catch (...) {
			Error("Error ocured while removing Game files");
		}
	}

}

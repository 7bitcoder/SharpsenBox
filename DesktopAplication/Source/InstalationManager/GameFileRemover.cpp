#include "GameFileRemover.hpp"
#include "Config.hpp"

namespace im {
	bool GameFileRemover::run() {
		try {
			std::filesystem::path gamePath = game_->gameDir.toStdString();
			for (auto& path : *toRemove_) {
				auto filePath = gamePath / path.toStdString();
				if (std::filesystem::exists(filePath))
					std::filesystem::remove(filePath);
			}
		} catch (...) {

		}
	}

}

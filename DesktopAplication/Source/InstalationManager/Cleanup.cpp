#include "Cleanup.hpp"
#include <filesystem>
#include "ObjectRepo.hpp"
#include "IConfig.hpp"


namespace im {
	bool Cleanup::run() {
		try {
			auto& downloadDir = bc::ObjectsRepository::getRepo().getConfig().getDownloadDir();
			for (auto& p : std::filesystem::recursive_directory_iterator(downloadDir)) {
				auto& path = p.path();
				std::filesystem::remove_all(path);
			}
		} catch (...) {
			errorStr_ = "Error ocured while removing temporary files";
			return false;
		}
		return true;
	}
}
#include "Cleanup.hpp"
#include <filesystem>
#include "IConfig.hpp"


namespace sb {
	void Cleanup::run() {
		try {
			auto& downloadDir = Component<IConfig>::get().getDownloadDir();
			for (auto& p : std::filesystem::recursive_directory_iterator(downloadDir)) {
				auto& path = p.path();
				std::filesystem::remove_all(path);
			}
		} catch (...) {
			error("Error ocured while removing temporary files");
		}
	}
}
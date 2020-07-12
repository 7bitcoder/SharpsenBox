#include "Cleanup.hpp"
#include <filesystem>
#include "Config.hpp"


namespace im {
	void Cleanup::run() {
		try {
			auto& downloadDir = cf::Config::getObject().getDownloadDir();
			for (auto& p : std::filesystem::recursive_directory_iterator(downloadDir)) {
				auto& path = p.path();
				std::filesystem::remove_all(path);
			}
			ended();
		} catch (...) {
			error(-1);
		}
	}
}
#include "UpdateInfo.hpp"

namespace sb {
	void UpdateInfo::Reset() {
		cancel_ = false;
		fullInstall_ = false;
		files_.clear();
		actualVersion_.clear();
		toUpdateVersion_.clear();

		downloadDir_.clear();
		installDir_.clear();
		pause.test_and_set();
		stop.test_and_set();
		resume.test_and_set();
	}

	UpdateInfo::UpdateInfo() {
		pause.test_and_set();
		stop.test_and_set();
		resume.test_and_set();
	}
}
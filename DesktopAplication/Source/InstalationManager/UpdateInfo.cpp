#include "UpdateInfo.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace im {
	void UpdateInfo::reset() {
		onlyDownload = false;
		cancel_ = false;
		fullInstall_ = false;
		emitMainAppDownload = false;
		files_.clear();
		actualVersion_.clear();
		toUpdateVersion_.clear();

		actualGame_ = nullptr;

		downloadDir_.clear();
		installDir_.clear();
	}

	UpdateInfo::UpdateInfo() {
		pause.test_and_set();
		stop.test_and_set();
		resume.test_and_set();
	}
}
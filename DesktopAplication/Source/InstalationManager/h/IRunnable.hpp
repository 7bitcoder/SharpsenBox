#pragma once
#include <QString>
#include "IInstalationManager.hpp"

namespace im {
	class UpdateInfo;
	struct IRunnable {
		virtual ~IRunnable() {};

		virtual bool run() = 0;
		virtual void reset() = 0;

		QString getErrorStr() {
			return errorStr_;
		}

		void init(IInstalationManager& im) {
			im_ = &im;
			updateInfo_ = &im.getUpdateInfo();
		}

		IInstalationManager* im_ = nullptr;
		UpdateInfo* updateInfo_ = nullptr;
		QString errorStr_;
	};
}
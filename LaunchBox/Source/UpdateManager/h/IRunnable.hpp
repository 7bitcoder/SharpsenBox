#pragma once
#include <QString>
#include "IUpdateManager.hpp"

namespace im {
	class UpdateInfo;
	struct IRunnable {
		virtual ~IRunnable() {};

		virtual bool run() = 0;
		virtual void reset() = 0;

		QString getErrorStr() {
			return errorStr_;
		}

		void init(IUpdateManager& im) {
			im_ = &im;
			updateInfo_ = &im.getUpdateInfo();
		}

		IUpdateManager* im_ = nullptr;
		UpdateInfo* updateInfo_ = nullptr;
		QString errorStr_;
	};
}
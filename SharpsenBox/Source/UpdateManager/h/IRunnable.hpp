#pragma once
#include <QString>
#include "IUpdateManager.hpp"

namespace im {
	class UpdateInfo;
	struct IRunnable {
		virtual ~IRunnable() {};

		virtual void run() = 0;
		virtual void reset() = 0;

		void init(IUpdateManager& im) {
			im_ = &im;
			updateInfo_ = &im.getUpdateInfo();
		}

		void error(const std::string& what) { throw std::exception(what.c_str()); }

		IUpdateManager* im_ = nullptr;
		UpdateInfo* updateInfo_ = nullptr;
	};
}
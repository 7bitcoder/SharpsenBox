#pragma once
#include <QString>
#include "IUpdateManager.hpp"

namespace sb {
	class UpdateInfo;
	struct IRunnable {
		virtual ~IRunnable() {};

		virtual void Run() = 0;
		virtual void Reset() = 0;

		void Init(IUpdateManager& im) {
			UpdateManager = &im;
			UpdateInfo = &im.getUpdateInfo();
		}

		void Error(const std::string& what) { throw std::exception(what.c_str()); }

		IUpdateManager* UpdateManager = nullptr;
		UpdateInfo* UpdateInfo = nullptr;
	};
}
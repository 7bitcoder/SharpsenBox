#pragma once
#include "Config.hpp"
#include "AppUpdaterManager.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::init() { register_.init(); }

	void Backend::registerComponents() {
		register_.createAndRegister<IConfig, Config>();
		register_.createAndRegister<IAppUpdaterManager, AppUpdaterManager>();
	}
}
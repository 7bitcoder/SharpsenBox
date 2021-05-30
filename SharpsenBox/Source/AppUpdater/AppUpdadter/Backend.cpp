#pragma once
#include "Config.hpp"
#include "AppUpdaterManager.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::Init() { register_.Init(); }

	void Backend::registerComponents() {
		register_.CreateAndRegister<IConfig, Config>();
		register_.CreateAndRegister<IAppUpdaterManager, AppUpdaterManager>();
	}
}
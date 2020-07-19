#pragma once
#include "Config.hpp"
#include "AppUpdaterManager.hpp"
#include "Backend.hpp"

namespace bc {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::init() { register_.init(); }

	void Backend::registerComponents() {
		register_.createAndRegister<cf::IConfig, cf::Config>();
		register_.createAndRegister<upd::IAppUpdaterManager, upd::AppUpdaterManager>();
	}
}
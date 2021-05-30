#pragma once
#include "Config.hpp"
#include "AppUpdaterManager.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : _Register(eng) {}

	void Backend::Init() { _Register.Init(); }

	void Backend::RegisterComponents() {
		_Register.CreateAndRegister<IConfig, Config>();
		_Register.CreateAndRegister<IAppUpdaterManager, AppUpdaterManager>();
	}
}
#pragma once
#include "GameManager.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"
#include "Config.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::Init() { register_.Init(); }

	void Backend::registerComponents() {
		register_.CreateAndRegister<IConfig, Config>();
		register_.CreateAndRegister<IDialog, Dialog>();
		register_.CreateAndRegister<ILoadingBar, LoadingBar>();
		register_.CreateAndRegister<IGameManager, GameManager>();
	}
}
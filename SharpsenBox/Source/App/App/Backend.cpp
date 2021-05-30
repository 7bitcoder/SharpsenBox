#pragma once
#include "GameManager.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"
#include "Config.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::init() { register_.init(); }

	void Backend::registerComponents() {
		register_.createAndRegister<IConfig, Config>();
		register_.createAndRegister<IDialog, Dialog>();
		register_.createAndRegister<ILoadingBar, LoadingBar>();
		register_.createAndRegister<IGameManager, GameManager>();
	}
}
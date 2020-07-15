#pragma once
#include "GameManager.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"
#include "Config.hpp"
#include "Backend.hpp"

namespace bc {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::init() { register_.init(); }

	CREATE_PTR(cf::IConfig);
	CREATE_PTR(dl::IDialog);
	CREATE_PTR(lb::ILoadingBar);
	CREATE_PTR(gm::IGameManager);

	void Backend::registerComponents() {
		register_.createAndRegister<cf::IConfig, cf::Config>();
		register_.createAndRegister<dl::IDialog, dl::Dialog>();
		register_.createAndRegister<lb::ILoadingBar, lb::LoadingBar>();
		register_.createAndRegister<gm::IGameManager, gm::GameManager>();
	}
}
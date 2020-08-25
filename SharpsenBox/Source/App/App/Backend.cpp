#pragma once
#include "GameManager.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"
#include "Config.hpp"
#include "Backend.hpp"

namespace bc {
	Backend::Backend(QQmlApplicationEngine* eng) : register_(eng) {}

	void Backend::init() { register_.init(); }

	void Backend::registerComponents() {
		register_.createAndRegister<cf::IConfig, cf::Config>();
		register_.createAndRegister<dl::IDialog, dl::Dialog>();
		register_.createAndRegister<lb::ILoadingBar, lb::LoadingBar>();
		register_.createAndRegister<gm::IGameManager, gm::GameManager>();
	}
}
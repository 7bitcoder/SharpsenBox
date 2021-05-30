#pragma once
#include "GameManager.hpp"
#include "LoadingBar.hpp"
#include "Dialog.hpp"
#include "Config.hpp"
#include "Backend.hpp"

namespace sb {
	Backend::Backend(QQmlApplicationEngine* eng) : _Register(eng) {}

	void Backend::Init() { _Register.Init(); }

	void Backend::RegisterComponents() {
		_Register.CreateAndRegister<IConfig, Config>();
		_Register.CreateAndRegister<IDialog, Dialog>();
		_Register.CreateAndRegister<ILoadingBar, LoadingBar>();
		_Register.CreateAndRegister<IGameManager, GameManager>();
	}
}
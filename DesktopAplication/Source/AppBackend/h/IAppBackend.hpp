#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"
#include <memory>

namespace cf {
	struct IConfig;
	class Config;
}
namespace lb {
	struct ILoadingBar;
	class LoadingBar;
}
namespace im {
	struct IInstalationManager;
	class InstalationManager;
}
namespace dl {
	struct IDialog;
	class Dialog;
}
namespace gm {
	struct IGameManager;
	class GameManager;
}

namespace bc {
	class Backend;
	struct IBackend {
	public:
		virtual ~IBackend() {}

		virtual void init(QQmlApplicationEngine* eng) = 0;

		virtual cf::IConfig& getConfig() = 0;
		virtual dl::IDialog& getDialog() = 0;
		virtual lb::ILoadingBar& getLoadingBar() = 0;
		virtual gm::IGameManager& getGameManager() = 0;
		virtual im::IInstalationManager& getInstalationManager() = 0;

	};
}
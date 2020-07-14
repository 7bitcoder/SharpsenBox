#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Config.hpp"
#include "InstalationManager.hpp"
#include "GameManager.hpp"
#include "Dialog.hpp"
#include "GameManager.hpp"
#include "LoadingBar.hpp"

namespace bc {

	void Backend::init(QQmlApplicationEngine* eng) {
		engine = eng;
		registerObjects();
		initializeObjects();
	}

	void Backend::registerObjects() {
		// register compoments in qml
		qmlRegisterObject(*config_);
		qmlRegisterObject(*dialog_);
		qmlRegisterObject(*lodingBar_);
		qmlRegisterObject(*gameManager_);

		// register components but dont expose to qml
		registerObject(*installationManager_);
	}

	void Backend::initializeObjects() {
		for (IComponent* object : objects_)
			object->init();
	}

	void Backend::registerObject(IComponent& object) {
		objects_.push_back(&object);
	}

	void Backend::qmlRegisterObject(IQmlComponent& object) {
		registerObject(object);
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
	}

	cf::IConfig& Backend::getConfig() { return *config_; }
	dl::IDialog& Backend::getDialog() { return *dialog_; }
	lb::ILoadingBar& Backend::getLoadingBar() { return *lodingBar_; }
	gm::IGameManager& Backend::getGameManager() { return *gameManager_; }
	im::IInstalationManager& Backend::getInstalationManager() { return *installationManager_; }

	Backend::~Backend() {}

	Backend::Backend() {
		config_.reset(new cf::Config);
		dialog_.reset(new dl::Dialog);
		lodingBar_.reset(new lb::LoadingBar);
		gameManager_.reset(new gm::GameManager);
		installationManager_.reset(new im::InstalationManager);
	}
}

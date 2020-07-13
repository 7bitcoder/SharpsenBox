#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "ObjectRepo.hpp"
#include "IConfig.hpp"
#include "IInstalationManager.hpp"
#include "IGameManager.hpp"
#include "IDialog.hpp"
#include "IGameManager.hpp"
#include "ILoadingBar.hpp"

namespace bc {

	Backend& Backend::getBakend() {
		static Backend backend;
		return backend;
	}

	void Backend::init(QQmlApplicationEngine* eng) {
		engine = eng;
		registerObjects();
		initializeObjects();
	}

	void Backend::registerObjects() {
		// register compoments in qml
		auto& repo = ObjectsRepository::getRepo();
		qmlRegisterObject(repo.getConfig());
		qmlRegisterObject(repo.getDialog());
		qmlRegisterObject(repo.getLoadingBar());
		qmlRegisterObject(repo.getGameManager());

		// register components but dont expose to qml
		registerObject(repo.getInstalationManager());
	}

	void Backend::initializeObjects() {
		for (IComponent* object : objects_)
			object->init();
	}

	IComponent& Backend::registerObject(IComponent& object) {
		objects_.push_back(&object);
		return object;
	}

	void Backend::qmlRegisterObject(IQmlComponent& object) {
		registerObject(object);
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
	}

	Backend::~Backend() {}
}

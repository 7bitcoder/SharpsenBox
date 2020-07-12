#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Dialog.hpp"
#include "InstalationManager.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include "GameManager.hpp"

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
		qmlRegisterObject<cf::Config>();
		qmlRegisterObject<dl::Dialog>();
		qmlRegisterObject<lb::LoadingBar>();
		qmlRegisterObject<gm::GameManager>();

		// register components but dont expose to qml
		registerObject<im::InstalationManager>();
	}

	void Backend::initializeObjects() {
		for (auto* object : objects_)
			object->init();
	}

	template <class T>
	T& Backend::registerObject() {
		auto& object = T::getObject();
		objects_.push_back(&object);
		return object;
	}

	template <class T>
	void Backend::qmlRegisterObject() {
		auto& object = registerObject<T>();
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
	}

	Backend::~Backend() {}
}

#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Dialog.hpp"
#include "InstalationManager.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include "GameManager.hpp"
#include "GameExecutor.hpp"



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
		registerObject<cf::Config>();
		registerObject<dl::Dialog>();
		registerObject<bb::InstalationManager>();
		registerObject<lb::LoadingBar>();
		registerObject<gm::GameManager>();
		registerObject<ge::GameExecutor>();
	}
	void Backend::initializeObjects() {
		for (auto* object : objects_)
			object->init();
	}
	template <class T>
	void Backend::registerObject() {
		auto& object = T::getObject();
		objects_.push_back(&object);
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
	}

	Backend::~Backend() {}
}

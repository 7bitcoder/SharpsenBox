#include <iostream>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <QQmlContext>
#include <QVariant>
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
		qmlRegisterObject<cf::Config>();
		qmlRegisterObject<dl::Dialog>();
		qmlRegisterObject<lb::LoadingBar>();
		qmlRegisterObject<gm::GameManager>();

		// register components but dont expose to qml
		registerObject<im::InstalationManager>();
	}

	void Backend::initializeObjects() {
		for (IComponent* object : objects_)
			object->init();
	}

	template<class T>
	T& Backend::registerObject() {
		auto* object = new T;
		objects_.push_back(object);
		return *object;
	}

	template<class T>
	void Backend::qmlRegisterObject() {
		auto& object = registerObject<T>();
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
	}

	Backend::~Backend() {}

	Backend::Backend() {}
}

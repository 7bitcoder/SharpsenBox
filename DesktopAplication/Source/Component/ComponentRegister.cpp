#include <iostream>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <QQmlContext>
#include <QVariant>
#include "ComponentRegister.hpp"

namespace bc {

	void ComponentRegister::init() {
		initializeObjects();
	}

	void ComponentRegister::initializeObjects() {
		for (IComponent* object : objects_)
			object->init();
	}

	void ComponentRegister::registerObject(IComponent& component) {
		objects_.push_back(&component);
		auto name = component.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &component);
	}

	ComponentRegister::~ComponentRegister() {}

	ComponentRegister::ComponentRegister(QQmlApplicationEngine* eng) { engine = eng; }
}

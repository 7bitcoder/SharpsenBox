#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "AppBackend.hpp"
#include "TreeModel.hpp"

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
		registerObject<dt::TreeModel>();
	}
	void Backend::initializeObjects() {}
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

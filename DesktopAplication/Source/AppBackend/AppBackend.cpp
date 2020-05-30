#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Button.hpp"
#include "AppUpdateChecker.hpp"
#include "BottomBar.hpp"



namespace bc {

	Backend& Backend::getBakend() {
		static Backend backend;
		return backend;
	}
	void Backend::init(QQmlApplicationEngine* eng) {
		engine = eng;
		registerObjects();
	}

	void Backend::registerObjects() {
		// ZIP macro changes [Class] to ["Class", new Class]
		registerObject(new Button);
		registerObject(new upd::AppUpdateChecker);
		registerObject(new bb::BottomBar);
	}

	void Backend::registerObject(IQmlObject* object) {
		auto name = object->getName();
		if (objects_.find(name) != objects_.end())
			throw std::exception("object already registered");
		objects_.insert({ name , object });
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, object);
	}

	QObject* Backend::getObject(std::string name) {
		auto it = objects_.find(name);
		if (it == objects_.end())
			throw std::exception("wrong object name");
		return it->second;
	}

	Backend::~Backend() {
		for (auto ptr : objects_)
			delete ptr.second;
	}
}
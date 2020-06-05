#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Button.hpp"
#include "InstalationManager.hpp"
#include "Config.hpp"
#include "LoadingBarManager.hpp"



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
		registerObject<cf::Config>();
		registerObject<Button>();
		registerObject<bb::LoadingBarManager>();
		registerObject<bb::InstalationManager>();
	}

	template <class T>
	void Backend::registerObject() {
		auto& object = T::getObject();
		object.init();
		auto name = object.getName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, &object);
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
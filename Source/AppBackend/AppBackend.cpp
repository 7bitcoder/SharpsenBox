#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Button.hpp"

#define ZIP(TYPE) #TYPE, new TYPE()

namespace bc {
	void Backend::init() {
		registerObjects();
	}

	void Backend::registerObjects() {
		registerObject(ZIP(Button));
	}

	void Backend::registerObject(std::string name, QObject* object) {
		objects_.push_back(object);
		engine.rootContext()->setContextProperty(name.insert(0, "_").c_str(), objects_.back());
	}
	Backend::~Backend() {
		for (auto ptr : objects_)
			delete ptr;
	}
}
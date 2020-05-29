#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Button.hpp"
#include "AppUpdateChecker.hpp"

#define ZIP(TYPE) #TYPE, new TYPE

namespace bc {
	QQmlApplicationEngine* Backend::engine = nullptr;

	void Backend::init() {
		registerObjects();
	}

	void Backend::registerObjects() {
		// ZIP macro changes [Class] to ["Class", new Class]
		registerObject(ZIP(Button));
		registerObject(ZIP(upc::AppUpdateChecker));
	}

	void Backend::registerObject(std::string name, QObject* object) {
		objects_.push_back(object);
		auto it = name.find_last_of(":");
		if (it != std::string::npos)
			name = name.substr(it + 1);
		name.insert(0, "_");
		auto cStr = name.c_str();
		engine->rootContext()->setContextProperty(cStr, objects_.back());
	}

	Backend::~Backend() {
		for (auto ptr : objects_)
			delete ptr;
	}
}
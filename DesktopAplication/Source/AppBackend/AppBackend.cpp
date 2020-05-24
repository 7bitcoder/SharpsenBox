#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "Button.hpp"

#define ZIP(TYPE) #TYPE, new TYPE()
#define REGISTER(CLASS) qmlRegisterType<CLASS>("_"#CLASS, 1, 0, "_"#CLASS); registerObject(ZIP(CLASS))

namespace bc {
	QQmlApplicationEngine* Backend::engine = nullptr;

	void Backend::init() {
		registerObjects();
	}

	void Backend::registerObjects() {
		REGISTER(Button);
		//registerObject(ZIP(Button));
	}

	void Backend::registerObject(std::string name, QObject* object) {
		objects_.push_back(object);

		engine->rootContext()->setContextProperty(name.insert(0, "_").c_str(), objects_.back());
	}
	Backend::~Backend() {
		for (auto ptr : objects_)
			delete ptr;
	}
}
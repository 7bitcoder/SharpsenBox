#include <iostream>
#include <QDebug>
#include <QQmlContext>
#include "AppBackend.hpp"
#include "AppBackend.hpp"
#include "TreeModel.hpp"
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qvariant.h>

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
		dt::TreeModel::setRoot("C:/Users/Sylwester/Desktop/asd asdsad");
		qmlRegisterType<dt::TreeModel>("custom.TreeModel", 1, 1, "TreeModel");
		//uc.setupModelData( ".", nullptr);
		//QFileSystemModel* filemodel = new QFileSystemModel;
		//filemodel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
		//filemodel->setNameFilterDisables(false);
		//filemodel->supportedDragActions();
		//filemodel->setRootPath("C:/Users/Sylwester/Desktop/TS");
		//
		//engine->rootContext()->setContextProperty("_TreeModel", filemodel);
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

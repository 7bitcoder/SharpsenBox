#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <fstream>
#include <iostream>
#include "Backend.hpp"

int main(int argc, char** argv) {
	//responsible is more efficient but animations are too fast
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	bc::Backend backend(&engine);
	backend.registerComponents();
	backend.init();

	engine.load("qrc:/AppUpdater.qml");
	//_sleep(10000);
	return app.exec();
}
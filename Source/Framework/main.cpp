#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qvariant.h>
#include "AppBackend.hpp"



int main(int argc, char** argv) {
	bc::Backend;
	QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	bc::Backend backend(engine);
	backend.init();
	engine.load("qrc:/LaunchBox.qml");
	return app.exec();
}
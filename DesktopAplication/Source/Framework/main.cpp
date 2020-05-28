#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qvariant.h>
#include "AppBackend.hpp"
#include "Button.hpp"

int main(int argc, char** argv) {
	//poprawia wydajno�� ale animacje s� za szybkie
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	engine.load("qrc:/LaunchBox.qml");
	bc::Backend backend(engine);
	backend.init();
	return app.exec();
}
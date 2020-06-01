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
	bc::Backend& backend = bc::Backend::getBakend();
	backend.init(&engine);
	engine.load("qrc:/LaunchBox.qml");
	//_sleep(10000);
	return app.exec();
}
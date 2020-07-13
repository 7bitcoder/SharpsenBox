#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qvariant.h>
#include "AppBackend.hpp"

int main(int argc, char** argv) {
	//responsible is more efficient but animations are too fast
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	bc::IBackend& backend = bc::Backend::getBackend();
	backend.init(&engine);
	engine.load("qrc:/LaunchBox.qml");
	//_sleep(10000);
	return app.exec();
}
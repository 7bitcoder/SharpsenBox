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
	bc::Backend& backend = bc::Backend::getBakend();
	backend.init(&engine);
	engine.load("qrc:/Pather.qml");
	//_sleep(10000);
	return app.exec();
}

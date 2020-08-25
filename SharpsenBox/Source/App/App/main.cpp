#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <QIcon>
#include "Backend.hpp"

int main(int argc, char** argv) {
	//responsible is more efficient but animations are too fast
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	app.setWindowIcon(QIcon("orbit2.ico"));
	QQmlApplicationEngine engine;

	bc::Backend backend(&engine);
	backend.registerComponents();
	backend.init();

	engine.load("qrc:/SharpsenBox.qml");
	return app.exec();
}

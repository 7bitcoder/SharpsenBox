#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>
#include "backend.h"


int main(int argc, char** argv) {
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QQmlContext* rootContext = engine.rootContext();
	rootContext->setContextProperty("WINDOW_WIDTH", 640);
	rootContext->setContextProperty("WINDOW_HEIGHT", 360);


	engine.load(QStringLiteral("LaunchBox.qml"));

	return app.exec();
}
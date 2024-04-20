#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <QIcon>
#include "Backend.hpp"

int main(int argc, char** argv)
{
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	app.setWindowIcon(QIcon("orbit2.ico"));
	QQmlApplicationEngine engine;
	// engine.addImportPath("qrc:/");

	sb::Backend backend(&engine);
	backend.RegisterComponents();
	backend.Init();

	engine.load("qrc:/qt/qml/gui/Gui/SharpsenBox.qml");
	return app.exec();
}

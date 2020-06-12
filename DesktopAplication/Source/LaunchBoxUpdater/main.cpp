#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include "AppUpdater.hpp"
#include "InstalationManager.hpp"
#include "ArchieveInstaller.hpp"

#include <fstream>
#include <iostream>

namespace {
    bool check_character(int value) {
        return value == 3;
    }
}
int main(int argc, char** argv) {
	//responsible is more efficient but animations are too fast
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	auto updater = new upd::AppUpdater();
	engine.rootContext()->setContextProperty("_AppUpdater", updater);
	auto& im = bb::InstalationManager::getObject();
	engine.rootContext()->setContextProperty("_InstalationManager", &im);
	engine.load("qrc:/AppUpdater.qml");
	//_sleep(10000);
	return app.exec();
    return 0;
}
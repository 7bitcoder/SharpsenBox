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
	//poprawia wydajnoœæ ale animacje s¹ za szybkie
	//QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	/*QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	auto updater = new upd::AppUpdater();
	engine.rootContext()->setContextProperty("_AppUpdater", updater);
	auto& im = bb::InstalationManager::getObject();
	engine.rootContext()->setContextProperty("_InstalationManager", &im);
	engine.load("qrc:/AppUpdater.qml");
	//_sleep(10000);
	return app.exec();*/

	bb::ArchieveInstaller im;
	im.run();


   //static constexpr size_t size = 8096;
   //char* buffer = new char[size];
   //int* ints = (int*)buffer;
   //size_t counter = 0;
   //
   //// create our stream, 
   //std::ifstream stream("android_build.zip", std::ios_base::binary);
   //while (stream) {
   //    // keep reading while the stream is valid
   //    stream.read(buffer, size);
   //    auto count = stream.gcount();
   //
   //    // we only want to go to the last valid integer
   //    // if we expect the file to be only integers,
   //    // we could do `assert(count % sizeof(int) == 0);
   //    // otherwise, we may have trailing characters
   //
   //    // if we have trailing characters, we may want to move them
   //    // to the front of the buffer....
   //    for (size_t i = 0; i < count; ++i, std::cout << "\n") {
   //        // false == 0, true == 1, so we can just add
   //        // if the value is 3
   //        std::cout << buffer[i];
   //    }
   //}
   //delete[] buffer;
    return 0;
}
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QQuickView>
#include <QQmlApplicationEngine>


int main(int argc, char** argv) {
	QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine("qrc:/LaunchBox.qml");
	return app.exec();
	// QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// QCoreApplication::setOrganizationName("QtExamples");
	// 
	// QGuiApplication app(argc, argv);
	// 
	// QQuickView view;
	// view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
	// view.setSource(QUrl("qrc:/LaunchBox.qml"));
	// if (view.status() == QQuickView::Error)
	// 	return -1;
	// view.setResizeMode(QQuickView::SizeRootObjectToView);
	// view.setFlags(Qt::FramelessWindowHint | Qt::Window);
	// view.show();
	// auto* object = view.rootObject();
	// return app.exec();
}
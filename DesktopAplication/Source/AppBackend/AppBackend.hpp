#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>

namespace bc {
	class Backend {
	public:
		/*
		Backend class - registers all objects that will be exported to qml
		*/
		explicit Backend(QQmlApplicationEngine& eng) { engine = &eng; }
		~Backend();

		void init();
		static QQmlApplicationEngine* engine;
	private:
		void registerObjects();

		void registerObject(std::string name, QObject* object);

		std::vector<QObject*> objects_;
	};
}
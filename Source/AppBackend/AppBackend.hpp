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
		explicit Backend(QQmlApplicationEngine& parent) : engine(parent) {}
		~Backend();
		void init();
	private:
		QQmlApplicationEngine& engine;
		void registerObjects();
		void registerObject(std::string name, QObject* object);
		std::vector<QObject*> objects_;
	};
}
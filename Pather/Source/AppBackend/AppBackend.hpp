#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IQmlObject.hpp"

// Backend class - registers all objects that will be exported to qml
namespace bc {
	class Backend {
	public:
		static Backend& getBakend();
		void init(QQmlApplicationEngine* eng);

	private:
		Backend() {};
		~Backend();
		QQmlApplicationEngine* engine;
		template <class T>
		void registerObject();
		void registerObjects();
		void initializeObjects();

		std::vector<IQmlObject*> objects_;
	};
}
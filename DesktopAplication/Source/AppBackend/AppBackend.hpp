#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"
#include "ObjectRepo.hpp"

// Backend class - registers all objects that will be exported to qml

namespace bc {
	class Backend {
	public:
		static Backend& getBakend();
		void init(QQmlApplicationEngine* eng);
	private:
		Backend() {};
		~Backend();

		void qmlRegisterObject(IQmlComponent& object);

		IComponent& registerObject(IComponent& object);

		void registerObjects();
		void initializeObjects();
		QQmlApplicationEngine* engine;
		std::vector<IComponent*> objects_;
	};
}
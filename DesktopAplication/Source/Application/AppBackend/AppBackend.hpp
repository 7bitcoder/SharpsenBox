#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"

// Backend class - registers all objects that will be exported to qml

namespace bc {
	class Backend {
	public:
		static Backend& getBackend() {
			static Backend bc;
			return bc;
		};

		void init(QQmlApplicationEngine* eng);

	private:
		Backend();
		~Backend();
		template<class T>
		void qmlRegisterObject();

		template<class T>
		T& registerObject();

		void registerObjects();
		void initializeObjects();
		QQmlApplicationEngine* engine = nullptr;
		std::vector<IComponent*> objects_;
	};
}
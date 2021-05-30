#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"

#define CREATE_PTR(ICOMPONENT) ICOMPONENT* Component< ICOMPONENT >::iComponent_ = nullptr;

namespace sb {
	class ComponentRegister {
	public:
		ComponentRegister(QQmlApplicationEngine* eng);
		~ComponentRegister();

		void init();

		template<class I, class O>
		void createAndRegister() { registerObject(ComponentCreator<I, O>::create()); };

	private:
		void registerObject(IComponent& component);
		void initializeObjects();

		QQmlApplicationEngine* engine = nullptr;
		std::vector<IComponent*> objects_;
	};
}
#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"

namespace sb
{
	class ComponentRegister
	{
	public:
		ComponentRegister(QQmlApplicationEngine *eng);
		~ComponentRegister();

		void Init();

		template <class I, class O>
		void CreateAndRegister() { RegisterObject(ComponentCreator<I, O>::Create()); };

	private:
		void RegisterObject(IComponent &component);
		void InitializeObjects();

		QQmlApplicationEngine *_Engine = nullptr;
		std::vector<IComponent *> _Objects;
	};
}
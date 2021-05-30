#include <iostream>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariant>
#include <QQmlContext>
#include <QVariant>
#include "ComponentRegister.hpp"

namespace sb
{

	void ComponentRegister::Init()
	{
		InitializeObjects();
	}

	void ComponentRegister::InitializeObjects()
	{
		for (IComponent *object : _Objects)
			object->Init();
	}

	void ComponentRegister::RegisterObject(IComponent &component)
	{
		_Objects.push_back(&component);
		auto name = component.GetName();
		name.insert(0, "_");
		auto cStr = name.c_str();
		_Engine->rootContext()->setContextProperty(cStr, &component);
	}

	ComponentRegister::~ComponentRegister() {}

	ComponentRegister::ComponentRegister(QQmlApplicationEngine *eng) { _Engine = eng; }
}

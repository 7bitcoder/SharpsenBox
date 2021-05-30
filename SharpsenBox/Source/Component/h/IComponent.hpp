#pragma once
#include <QObject>
#include <string>
#include <iostream>

#define TYPENAME(TYPE) #TYPE

namespace sb
{

	template <class I>
	class Component
	{
	public:
		static I &Get()
		{
			if (!_IComponent)
				throw std::exception("Component is not set");
			return *_IComponent;
		}

	protected:
		Component() {}
		virtual ~Component() {}
		inline static I *_IComponent = nullptr;
	};

	template <class I, class O>
	class ComponentCreator : public Component<I>
	{
	public:
		static O &Create()
		{
			static O component;
			Component<I>::_IComponent = static_cast<I *>(&component);
			return component;
		}

	private:
		ComponentCreator() {}
		~ComponentCreator() {}
	};

	struct IComponent : public QObject
	{
		virtual void Init() = 0;
		virtual std::string GetName() = 0;
		virtual void Update() = 0;
		virtual ~IComponent(){};
	};
}
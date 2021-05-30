#pragma once
#include <QObject>
#include <string>
#include <iostream>

#define TYPENAME(TYPE) #TYPE


namespace sb {

	template<class I>
	class Component {
	public:
		static I& get() {
			if (!iComponent_)
				throw std::exception("Component is not set");
			return *iComponent_;
		}
	protected:
		Component() {}
		virtual ~Component() {}
		inline static I* iComponent_ = nullptr;
	};

	template<class I, class O>
	class ComponentCreator : public Component<I> {
	public:
		static O& create() {
			static O component;
			Component<I>::iComponent_ = static_cast<I*>(&component);
			return component;
		}
	private:
		ComponentCreator() {}
		~ComponentCreator() {}
	};

	struct IComponent : public QObject {
		virtual void init() = 0;
		virtual std::string getName() = 0;
		virtual void update() = 0;
		virtual ~IComponent() {};
	};
}
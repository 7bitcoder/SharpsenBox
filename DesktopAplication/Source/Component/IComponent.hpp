#pragma once
#include <QObject>
#include <string>
#include <iostream>

#define TYPENAME(TYPE) #TYPE

namespace bc {

	template<class T>
	class Get {
	public:
		static T& get() {
			if (!component_)
				throw std::exception("Component is not set");
			return *component_;
		}
	protected:
		static T* component_;
		Get(T* component) { component_ = component; }
		~Get() {}

	};

	struct IComponent {
		virtual void init() = 0;
		virtual std::string getName() = 0;
		virtual void update() = 0;
		virtual ~IComponent() {};
	};

	struct IQmlComponent : public IComponent, public QObject {
		virtual ~IQmlComponent() {};
	};
}
#pragma once
#include <QObject>
#include <string>

#define TYPENAME(TYPE) #TYPE

namespace bc {

	template <class T>
	struct IStaticGet {
		static T& getObject() {
			static T st;
			return st;
		}
		virtual ~IStaticGet() {};
	};

	struct IInitializable {
		virtual void init() = 0;
		virtual ~IInitializable() {};
	};

	struct IObject : public IInitializable {
		virtual std::string getName() = 0;
		virtual void update() = 0;
		virtual ~IObject() {};
	};

	template <class T>
	struct IComponent : public IObject, public IStaticGet<T> {
		virtual ~IComponent() {};
	};

	template <class T>
	struct IQmlComponent : public IComponent<T>, public QObject {
		virtual ~IQmlComponent() {};
	};
}
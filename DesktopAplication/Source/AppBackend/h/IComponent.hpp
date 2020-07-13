#pragma once
#include <QObject>
#include <string>

#define TYPENAME(TYPE) #TYPE

namespace bc {

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
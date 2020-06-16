#pragma once
#include <QObject>
#include <string>

#define TYPENAME(TYPE) #TYPE

namespace bc {
	struct IQmlObject : public QObject {
		virtual std::string getName() = 0;
		virtual void update() = 0;
		virtual void init() = 0;
		virtual ~IQmlObject() {};
	};
}
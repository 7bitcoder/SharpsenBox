#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace sb {
	struct IDialog : public  IComponent {
	public:
		enum Type {NONE=0, INFO, INSTALL, DIALOG};
		IDialog() {};
		virtual ~IDialog() {};

		// interface
		virtual void setType(Type t) = 0;
		virtual void setInfo(QString str) = 0;
		virtual void show() = 0;
		virtual bool getValue() = 0;
		virtual void setCallback(const std::function<bool(bool)>& cb) = 0;

	};
}

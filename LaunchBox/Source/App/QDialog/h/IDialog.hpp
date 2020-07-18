#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace dl {
	struct IDialog : public  bc::IComponent {
	public:
		enum Type {NONE=0, INFO, INSTALL, DIALOG};
		IDialog() {};
		virtual ~IDialog() {};

		// interface
		virtual void setType(Type t) = 0;
		virtual void setInfo(QString str) = 0;
		virtual void show() = 0;
		virtual bool getValue() = 0;
		virtual void setCallback(const std::function<void(bool)>& cb) = 0;

	};
}

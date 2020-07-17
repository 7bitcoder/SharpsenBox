#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace dl {
	struct IDialog : public  bc::IComponent {
	public:
		IDialog() {};
		virtual ~IDialog() {};

		// interface
		virtual void setInfo(QString str) = 0;
		virtual void show() = 0;
		virtual bool getValue() = 0;
		virtual void setCallback(const std::function<void(bool)>& cb) = 0;

	};
}

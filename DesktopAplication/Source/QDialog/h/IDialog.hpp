#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace dl {
	struct IDialog : public  bc::IQmlComponent {
	public:
		IDialog() {};
		virtual ~IDialog() {};

		// interface
		virtual void setInfo(QString str) = 0;
		virtual void show() = 0;
		virtual bool getValue() = 0;

	};
}
//Q_DECLARE_INTERFACE(dl::IDialog, TYPENAME(dl::IDialog))
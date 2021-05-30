#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace sb
{
	struct IDialog : public IComponent
	{
	public:
		enum Type
		{
			NONE = 0,
			INFO,
			INSTALL,
			DIALOG
		};
		IDialog(){};
		virtual ~IDialog(){};

		// interface
		virtual void SetType(Type t) = 0;
		virtual void SetInfo(QString str) = 0;
		virtual void Show() = 0;
		virtual bool GetValue() = 0;
		virtual void SetCallback(const std::function<bool(bool)> &cb) = 0;
	};
}

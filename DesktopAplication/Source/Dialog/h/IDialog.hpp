#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IComponent.hpp"

namespace dl {
	struct IDialog : public  bc::IQmlComponent {
		Q_OBJECT
	public:
		IDialog() {};
		virtual ~IDialog() {};

		// interface
		virtual void setInfo(QString str) = 0;
		virtual void show() = 0;
		virtual bool getValue() = 0;

		// QML Propetries
		Q_PROPERTY(bool showDialog READ getShowDialog NOTIFY showDialogChanged);
		Q_PROPERTY(QString info READ getInfo);

		//QMl invoklabes
		virtual Q_INVOKABLE void dialog(bool value) = 0;
		virtual Q_INVOKABLE bool getShowDialog() = 0;
		virtual Q_INVOKABLE QString getInfo() = 0;

	signals:
		virtual void dialogTriggered(bool value) = 0;
		virtual void showDialogChanged() = 0;
	};
}
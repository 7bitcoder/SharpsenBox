#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>

namespace bc {
	class Button : public QObject {
		Q_OBJECT

	public:
		// QObjects are expected to support a parent/child hierarchy.  I've modified
		// the constructor to match the standard.
		Button();

	public slots:
		// This method needs to take either a QString or a const reference to one.
		// (QML doesn't support returning values via the parameter list.)
		void buttonClicked(const QString& in);
	private:
		QObject* textArea;
	};
}
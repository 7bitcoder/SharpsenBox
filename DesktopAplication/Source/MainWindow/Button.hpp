#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IQmlObject.hpp"

namespace bc {
	class Button : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Button& getObject() {
			static Button uc;
			return uc;
		}
		//Q_INVOKABLE QString launch(const QString& program);
		Q_INVOKABLE QString sayHello() const;
		Q_PROPERTY(QString text MEMBER string READ sayHello NOTIFY textChanged);
		// QObjects are expected to support a parent/child hierarchy.  I've modified
		// the constructor to match the standard.
		std::string getName() { return TYPENAME(Button); }
		void update() {}
		void init() override {}

	private:
		Button();
		virtual ~Button() {};
	public slots:
		// This method needs to take either a QString or a const reference to one.
		// (QML doesn't support returning values via the parameter list.)
		void buttonClicked(const QString& in);
	signals:
		void textChanged();
	private:
		QString string = "hello nygger";
		int value = 0;
	};
}
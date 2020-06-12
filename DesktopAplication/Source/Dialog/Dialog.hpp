#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IQmlObject.hpp"

namespace dl {
	class Dialog : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Dialog& getObject() {
			static Dialog uc;
			return uc;
		}

		// IQmlObject intefrace

		std::string getName() { return TYPENAME(Dialog); }
		void update() {}
		void init() override {}

		// interface
		void setInfo(QString str) { info_ = str; }
		void show() { show_ = true;  showDialogChanged(); }
		bool getValue() { return value_; }

		// QML Propetries
		Q_PROPERTY(bool showDialog READ getShowDialog NOTIFY showDialogChanged);
		Q_PROPERTY(QString info READ getInfo);


		Q_INVOKABLE void dialog(bool value);
		Q_INVOKABLE bool getShowDialog() { return show_; }
		Q_INVOKABLE QString getInfo() {	return info_; }

	private:
		Dialog() {};
		virtual ~Dialog() {};
	public slots:
	signals:
		void dialogTriggered(bool value);
		void showDialogChanged();
	private:
		QString info_ = "";
		bool show_ = false;
		bool value_ = false;
	};
}
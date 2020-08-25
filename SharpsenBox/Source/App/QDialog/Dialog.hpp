#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <iostream>
#include "IDialog.hpp"
#include "IComponent.hpp"

namespace dl {
	class Dialog final : public IDialog {
		Q_OBJECT;
	public:
		Dialog();
		virtual ~Dialog() {};

		// IQmlObject intefrace
		std::string getName() final { return TYPENAME(Dialog); }
		void update() final {}
		void init() final {}

		// interface
		void setType(Type t) { type_ = t; }
		void setInfo(QString str) final { info_ = str; }
		void show() final { showDialog(); }
		bool getValue() final { return value_; }

		void setCallback(const std::function<bool(bool)>& cb) final { callback_ = cb; }

		// QML Propetries
		Q_PROPERTY(int dialogType READ getDialogType NOTIFY showDialog);

		//QMl invoklabes
	public slots:
		void dialog(bool value);
		int getDialogType() { return static_cast<int>(type_); }
		QString getInfo() { return info_; }
	signals:
		void dialogTriggered(bool value);
		void showDialog();

	private:
		void resetCallback();
	private:
		QString info_ = "";
		Type type_ = NONE;
		bool value_ = false;
		std::function<bool(bool)> callback_ = nullptr;
	};
}
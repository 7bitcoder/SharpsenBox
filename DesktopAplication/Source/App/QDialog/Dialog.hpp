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
		void setInfo(QString str) final { info_ = str; }
		void show() final { show_ = true;  showDialogChanged(); }
		bool getValue() final { return value_; }

		void setCallback(const std::function<void(bool)>& cb) { callback_ = cb; }

		// QML Propetries
		Q_PROPERTY(bool showDialog READ getShowDialog NOTIFY showDialogChanged);
		Q_PROPERTY(QString info READ getInfo);

		//QMl invoklabes
	public slots:
		void dialog(bool value);
		bool getShowDialog() { return show_; }
		QString getInfo() { return info_; }
	signals:
		void dialogTriggered(bool value);
		void showDialogChanged();

	private:
		void resetCallback();
	private:
		QString info_ = "";
		bool show_ = false;
		bool value_ = false;
		std::function<void(bool)> callback_ = nullptr;
	};
}
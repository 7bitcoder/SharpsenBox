#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "IDialog.hpp"

namespace dl {
	class Dialog final : public IDialog {
		Q_OBJECT
	public:
		Dialog() {};
		virtual ~Dialog() {};

		// IQmlObject intefrace
		std::string getName() final { return TYPENAME(Dialog); }
		void update() final {}
		void init() final {}

		// interface
		void setInfo(QString str) final { info_ = str; }
		void show() final { show_ = true;  showDialogChanged(); }
		bool getValue() final { return value_; }

		//QMl invoklabes
		Q_INVOKABLE void dialog(bool value) final;
		Q_INVOKABLE bool getShowDialog() final { return show_; }
		Q_INVOKABLE QString getInfo() final { return info_; }

	public slots:
	signals:
		void dialogTriggered(bool value) final;
		void showDialogChanged() final;
	private:
		QString info_ = "";
		bool show_ = false;
		bool value_ = false;
	};
}
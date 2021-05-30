#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <iostream>
#include "IDialog.hpp"
#include "IComponent.hpp"

namespace sb
{
	class Dialog final : public IDialog
	{
		Q_OBJECT;

	public:
		Dialog();
		virtual ~Dialog();

		// IQmlObject intefrace
		std::string GetName() final;
		void Update() final;
		void Init() final;

		// interface
		void SetType(Type t) final;
		void SetInfo(QString str) final;
		void Show() final;
		bool GetValue() final;

		void SetCallback(const std::function<bool(bool)> &cb) final;

		// QML Propetries
		Q_PROPERTY(int dialogType READ GetDialogType NOTIFY ShowDialog);

		//QMl invoklabes
	public slots:
		void Resolve(bool value);
		int GetDialogType();
		QString GetInfo();
	signals:
		void DialogTriggered(bool value);
		void ShowDialog();

	private:
		void ResetCallback();

	private:
		QString _Info = "";
		Type _Type = NONE;
		bool _Value = false;
		std::function<bool(bool)> _Callback = nullptr;
	};
}
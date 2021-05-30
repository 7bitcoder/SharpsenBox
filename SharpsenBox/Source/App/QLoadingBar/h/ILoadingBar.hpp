#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IUpdateManager.hpp"
#include <filesystem>

namespace sb
{
	struct ILoadingBar : public IComponent
	{
	public:
		virtual ~ILoadingBar(){};

		virtual void SetTotal(double tot) = 0;
		virtual void SetActual(double act) = 0;
		virtual void SetProgress(double prog) = 0;
		virtual void SetSpeed(double sp) = 0;
		virtual void SetError(const QString &str) = 0;
		virtual void SetState(IUpdateManager::State st) = 0;
		virtual void SetVisibleState(IUpdateManager::VisibleState st) = 0;
		virtual void SetUninstallMode(bool un) = 0;

		virtual void Reset() = 0;
		virtual void Paused() = 0;
		virtual void Resumed() = 0;
	};
}

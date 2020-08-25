#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IUpdateManager.hpp"
#include <filesystem>

namespace lb {
	struct ILoadingBar : public bc::IComponent {
	public:
		virtual ~ILoadingBar() {};

		virtual void setTotal(double tot) = 0;
		virtual void setActual(double act) = 0;
		virtual void setProgress(double prog) = 0;
		virtual void setSpeed(double sp) = 0;
		virtual void setError(const QString& str) = 0;
		virtual void setState(im::IUpdateManager::State st) = 0;
		virtual void setVisibleState(im::IUpdateManager::VisibleState st) = 0;
		virtual void setUninstallMode(bool un) = 0;

		virtual void reset() = 0;
		virtual void paused() = 0;
		virtual void resumed() = 0;
	};
}

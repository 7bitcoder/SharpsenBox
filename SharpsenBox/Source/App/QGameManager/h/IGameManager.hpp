#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"

namespace sb
{
	struct IGameManager : public IComponent
	{
	public:
		virtual ~IGameManager(){};

		// inferface
		virtual void Lock() = 0;
		virtual void UnLock() = 0;
		virtual bool Uninstall(bool dialogValue) = 0;

		virtual void LockChanged() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Stop() = 0;
	};
}

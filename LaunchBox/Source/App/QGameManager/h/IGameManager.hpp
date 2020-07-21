#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"

namespace gm {
	struct IGameManager : bc::IComponent {
	public:
		virtual ~IGameManager() {};

		// inferface
		virtual void lock() = 0;
		virtual void unLock() = 0;
		virtual bool uninstall(bool dialogValue) = 0;

		virtual	void lockChanged() = 0;

		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual void stop() = 0;
	};
}

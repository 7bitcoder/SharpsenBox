#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"

namespace gm {
	class GameUninstaller;
	struct IGameManager : bc::IQmlComponent {
		Q_OBJECT
	public:
		virtual ~IGameManager() {};
		IGameManager();

		// inferface
		virtual void lock() = 0;
		virtual void unLock() = 0;

		// Qml properties
		Q_PROPERTY(int lock READ getLock NOTIFY lockChanged);

		// QMl invokables
		Q_INVOKABLE virtual void installGame(int id, QString path, bool shortcut) = 0;;
		Q_INVOKABLE virtual bool getLock() = 0;
		Q_INVOKABLE virtual void unistallRequest(int id) = 0;
		Q_INVOKABLE virtual void checkAutoUpdate(int id) = 0;
		Q_INVOKABLE virtual void update(int id) = 0;
		Q_INVOKABLE virtual void runGame(int id) = 0;

	public slots:
		void virtual uninstallation(int id) = 0;
		void virtual uninstall(bool dialogValue) = 0;
	signals:
		void virtual lockChanged() = 0;
	};
}

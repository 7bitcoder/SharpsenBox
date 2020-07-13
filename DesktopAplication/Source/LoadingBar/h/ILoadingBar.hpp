#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include <filesystem>
#include "LoadingBarStates.hpp"

namespace lb {
	struct ILoadingBar : public bc::IQmlComponent {
		Q_OBJECT
	public:
		virtual ~ILoadingBar() {};
		ILoadingBar() {};

		virtual void reset() = 0;
		//QML Propetries
		Q_PROPERTY(double speed READ getSpeed); //download speed B/s
		Q_PROPERTY(double actual READ getActual); //actual downloaded MB
		Q_PROPERTY(double total READ getTotal); // total MB to download
		Q_PROPERTY(double progress READ getProgress); // total MB to download

		Q_PROPERTY(int state READ getState NOTIFY stateChanged);
		Q_PROPERTY(int visibleState READ getVisibleState NOTIFY visibleStateChanged);

		Q_PROPERTY(QString errorString READ getErrorString NOTIFY errorChanged);

		Q_PROPERTY(int uninstall READ getUninstall);

		//QMl invoklabes
		Q_INVOKABLE virtual double getSpeed() const = 0;
		Q_INVOKABLE virtual double getActual() const = 0;
		Q_INVOKABLE virtual double getTotal() const = 0;
		Q_INVOKABLE virtual double getProgress() const = 0;

		Q_INVOKABLE virtual int getState() const = 0;
		Q_INVOKABLE virtual int getVisibleState() const = 0;

		Q_INVOKABLE virtual QString getErrorString() const = 0;

		Q_INVOKABLE virtual bool getUninstall() const = 0;

		Q_INVOKABLE virtual void pause() const = 0;
		Q_INVOKABLE virtual void resume() const = 0;
		Q_INVOKABLE virtual void stop() const = 0;

	public slots:
		virtual void setTotal(double tot) = 0;
		virtual void setActual(double act) = 0;
		virtual void setProgress(double prog) = 0;
		virtual void setSpeed(double sp) = 0;
		virtual void setError(const QString& str) = 0;
		virtual void setState(State st) = 0;
		virtual void setVisibleState(VisibleState st) = 0;
		virtual void setUninstallMode(bool un) = 0;
	signals:
		virtual void stateChanged() = 0;
		virtual void visibleStateChanged() = 0;

		virtual void errorChanged() = 0;
		virtual void notifyEnded() = 0;
		virtual void getProgress(qint64 actual) = 0;
	};
}

#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>

namespace upc {
	class AppUpdateChecker : public QObject {
		Q_OBJECT
	public:
		AppUpdateChecker() {}
		virtual ~AppUpdateChecker() {};

	public slots:

		void checkForUpdates();
	signals:
	private:
	};
}
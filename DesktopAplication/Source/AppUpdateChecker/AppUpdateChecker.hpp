#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"


namespace upd {
	class AppUpdateChecker : public bc::IQmlObject {
		Q_OBJECT
	public:
		virtual ~AppUpdateChecker() {};
		AppUpdateChecker() {}

		// implementation IQmlObject
		void update() override;
		std::string getName() override;
		void init() override {}


		//QML Propetries
		//Q_PROPERTY(qint64 progress READ getProgress NOTIFY progresChanged);
		//QMl invoklabes
		//Q_INVOKABLE qint64 getProgress() const;


	public slots:
		//void checkForUpdates();
	signals:
		//void progresChanged();
	private:
		qint64 progress_ = 0;
		qint64 total_ = 0;
	};
}
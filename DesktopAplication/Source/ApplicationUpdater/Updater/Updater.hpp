#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IConfig.hpp"
#include "UpdateManager.hpp"
#include <filesystem>


namespace upd {
	class AppUpdater : public QObject {
		Q_OBJECT
	public:

		Q_PROPERTY(QString statusStr READ getStateStr);
		Q_PROPERTY(int progress READ getProgress NOTIFY progressChanged);
		Q_PROPERTY(int updateState READ getState NOTIFY stateChanged);

		Q_INVOKABLE QString getStateStr();
		Q_INVOKABLE int getProgress();
		Q_INVOKABLE int getState();
		Q_INVOKABLE void checkForUpdates();

		AppUpdater();
		virtual ~AppUpdater() {};
	private:
	public slots:
		void updateInstalled(const QString& version);
		void updateProgress(double progress);
		void errorCatched(const QString& what);
		void updateStatus(im::State state);
	signals:
		void stateChanged();
		void progressChanged();

	private:
		cf::IConfig& cf_;
		im::UpdateManager im_;
		QString statusStr_;
		im::State state_ = im::State::NONE;

		double progress_;
	};
}

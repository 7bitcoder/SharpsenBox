#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IConfig.hpp"
#include "UpdateManager.hpp"
#include "IAppUpdaterManager.hpp"
#include <filesystem>
#include <string>


namespace upd {
	class AppUpdaterManager : public IAppUpdaterManager {
		Q_OBJECT
	public:

		std::string getName() override { return TYPENAME(IAppUpdaterManager); }
		void init() override {}
		void update() override {}

		Q_PROPERTY(QString statusStr READ getStateStr);
		Q_PROPERTY(int progress READ getProgress NOTIFY progressChanged);
		Q_PROPERTY(int updateState READ getState NOTIFY stateChanged);

		Q_INVOKABLE QString getStateStr();
		Q_INVOKABLE int getProgress();
		Q_INVOKABLE int getState();
		Q_INVOKABLE void checkForUpdates();

		AppUpdaterManager();
		virtual ~AppUpdaterManager() {};
	private:
	public slots:
		void updateInstalled(const QString& version);
		void updateProgress(double progress);
		void errorCatched(const QString& what);
		void updateStatus(im::IUpdateManager::State state);
	signals:
		void stateChanged();
		void progressChanged();

	private:
		cf::IConfig& cf_;
		im::UpdateManager im_;
		QString statusStr_;
		im::IUpdateManager::State state_ = im::IUpdateManager::State::NONE;

		double progress_;
	};
}

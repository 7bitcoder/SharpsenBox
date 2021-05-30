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

namespace sb
{
	class AppUpdaterManager : public IAppUpdaterManager
	{
		Q_OBJECT
	public:
		std::string GetName() override { return TYPENAME(AppUpdaterManager); }
		void Init() override {}
		void Update() override {}

		Q_PROPERTY(QString statusStr READ GetStateStr);
		Q_PROPERTY(int progress READ GetProgress NOTIFY ProgressChanged);
		Q_PROPERTY(int updateState READ GetState NOTIFY StateChanged);

		Q_INVOKABLE QString GetStateStr();
		Q_INVOKABLE int GetProgress();
		Q_INVOKABLE int GetState();
		Q_INVOKABLE void CheckForUpdates();

		AppUpdaterManager();
		virtual ~AppUpdaterManager();

	private:
	public slots:
		void UpdateInstalled(const QString &version);
		void UpdateProgress(double progress);
		void ErrorCatched(const QString &what);
		void UpdateSt(int status);
	signals:
		void StateChanged();
		void ProgressChanged();

	private:
		void UpdateStatus(IUpdateManager::State state);
		IConfig &_Config;
		UpdateManager _UpdateManager;
		QString _StatusStr;
		IUpdateManager::State _State = IUpdateManager::State::NONE;

		double _Progress;
	};
}

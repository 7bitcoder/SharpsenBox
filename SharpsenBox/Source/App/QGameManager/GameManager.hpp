#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IGameManager.hpp"
#include "ILoadingBar.hpp"
#include "UpdateManager.hpp"

namespace sb
{
	class GameUninstaller;
	class GameManager final : public IGameManager
	{
		Q_OBJECT
	public:
		virtual ~GameManager();
		GameManager();

		// implementation IQmlComponent
		void Update() final{};
		std::string GetName() final;
		void Init() final;

		// inferface
		void Lock() final
		{
			_Lock = true;
			LockChanged();
		}
		void UnLock() final
		{
			_Lock = false;
			LockChanged();
		}
		bool Uninstall(bool dialogValue) final;
		void InstallGame(bool value);

		void Pause() final;
		void Resume() final;
		void Stop() final;

		// Qml properties
		Q_PROPERTY(int lock READ GetLock NOTIFY LockChanged);

		// QMl invokables
		Q_INVOKABLE void InstallGameRequest(int id);
		Q_INVOKABLE void SetInstallProperties(int id, QString path, bool shortcut)
		{
			_GameId = id;
			_Shortcut = _Shortcut;
			_Path = path;
		};
		Q_INVOKABLE bool GetLock() { return _Lock; }
		Q_INVOKABLE void UnistallRequest(int id);
		Q_INVOKABLE void CheckAutoUpdate(int id);
		Q_INVOKABLE void Update(int id);
		Q_INVOKABLE void RunGame(int id);

	public slots:
		void Uninstallation(int id);
		void ErrorEmit(const QString &errorStr);
		void UpdateProgress(double prog);

		void SetTotalLb(double tot);
		void SetActualLb(double act);
		void SetSpeedLb(double sp);
		void SetStateLb(int st);
		void SetVisibleStateLb(int st);
		void SetUninstallModeLb(bool un);

		void Paused() { _LoadingBar->Paused(); };
		void Resumed() { _LoadingBar->Resumed(); };
	signals:
		void LockChanged();

	private:
		bool CheckProcess();
		bool _Lock = false;
		int _GameId;
		QString _Path;
		bool _Shortcut;
		GameUninstaller *_Uninstaller;
		UpdateManager _UpdateManager;
		ILoadingBar *_LoadingBar;
	};
}

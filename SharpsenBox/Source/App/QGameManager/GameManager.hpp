#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IGameManager.hpp"
#include "ILoadingBar.hpp"
#include "UpdateManager.hpp"

namespace sb {
	class GameUninstaller;
	class GameManager final :public IGameManager {
		Q_OBJECT
	public:
		virtual ~GameManager();
		GameManager();

		// implementation IQmlComponent
		void Update() final {};
		std::string GetName() final;
		void Init() final;

		// inferface
		void lock() final { lock_ = true; lockChanged(); }
		void unLock() final { lock_ = false; lockChanged(); }
		bool uninstall(bool dialogValue) final;
		void installGame(bool value);

		void pause() final;
		void resume() final;
		void stop() final;

		// Qml properties
		Q_PROPERTY(int lock READ getLock NOTIFY lockChanged);

		// QMl invokables
		Q_INVOKABLE void installGameRequest(int id);
		Q_INVOKABLE void setInstallProperties(int id, QString path, bool shortcut) { gameId_ = id; shortcut_ = shortcut_; path_ = path; };
		Q_INVOKABLE bool getLock() { return lock_; }
		Q_INVOKABLE void unistallRequest(int id);
		Q_INVOKABLE void checkAutoUpdate(int id);
		Q_INVOKABLE void Update(int id);
		Q_INVOKABLE void runGame(int id);

	public slots:
		void uninstallation(int id);
		void errorEmit(const QString& errorStr);
		void updateProgress(double prog);

		void setTotalLb(double tot);
		void setActualLb(double act);
		void setSpeedLb(double sp);
		void setStateLb(int st);
		void setVisibleStateLb(int st);
		void setUninstallModeLb(bool un);

		void paused() { lb_->Paused(); };
		void resumed() { lb_->Resumed(); };
	signals:
		void lockChanged();


	private:
		bool checkProcess();
		bool lock_ = false;
		int gameId_;
		QString path_;
		bool shortcut_;
		GameUninstaller* uninstaller_;
		UpdateManager UpdateManager;
		ILoadingBar* lb_;
	};
}

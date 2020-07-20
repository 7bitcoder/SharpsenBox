#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IGameManager.hpp"
#include "ILoadingBar.hpp"
#include "UpdateManager.hpp"

namespace gm {
	class GameUninstaller;
	class GameManager final :public IGameManager {
		Q_OBJECT
	public:
		virtual ~GameManager() {};
		GameManager();

		// implementation IQmlComponent
		void update() final {};
		std::string getName() final;
		void init() final;

		// inferface
		void lock() final { lock_ = true; lockChanged(); }
		void unLock() final { lock_ = false; lockChanged(); }
		bool uninstall(bool dialogValue) final;
		void installGame(bool value);

		// Qml properties
		Q_PROPERTY(int lock READ getLock NOTIFY lockChanged);

		// QMl invokables
		Q_INVOKABLE void installGameRequest(int id);
		Q_INVOKABLE void setInstallProperties(int id, QString path, bool shortcut) { gameId_ = id; shortcut_ = shortcut_; path_ = path; };
		Q_INVOKABLE bool getLock() { return lock_; }
		Q_INVOKABLE void unistallRequest(int id);
		Q_INVOKABLE void checkAutoUpdate(int id);
		Q_INVOKABLE void update(int id);
		Q_INVOKABLE void runGame(int id);

	public slots:
		void uninstallation(int id);
		void errorEmit(const QString& errorStr);
		void updateProgress(double prog);

		void setTotalLb(double tot);
		void setActualLb(double act);
		void setSpeedLb(double sp);
		void setStateLb(int st) { lb_->setState(im::IUpdateManager::State(st)); }
		void setVisibleStateLb(int st) { lb_->setVisibleState(im::IUpdateManager::VisibleState(st)); }
		void setUninstallModeLb(bool un);
		void gameUpdateEnded();
	signals:
		void lockChanged();


	private:
		bool checkProcess();
		bool lock_ = false;
		int gameId_;
		QString path_;
		bool shortcut_;
		GameUninstaller* uninstaller_;
		im::UpdateManager im_;
		lb::ILoadingBar* lb_;
	};
}

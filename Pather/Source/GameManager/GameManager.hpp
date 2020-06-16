#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
namespace gm {
	class GameUninstaller;
	class GameManager : public bc::IQmlObject {
		Q_OBJECT
	public:
		static GameManager& getObject() {
			static GameManager uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override {};
		std::string getName() override;
		void init() override;

		// inferface
		void lock() { lock_ = true; lockChanged(); }
		void unLock() { lock_ = false; lockChanged(); }

		// Qml properties
		Q_PROPERTY(int lock READ getLock NOTIFY lockChanged);

		// QMl invokables
		Q_INVOKABLE void installGame(int id, QString path, bool shortcut);
		Q_INVOKABLE bool getLock() { return lock_; }
		Q_INVOKABLE void unistallRequest(int id);
		Q_INVOKABLE void checkAutoUpdate(int id);
		Q_INVOKABLE void update(int id);

	public slots:
		void uninstallation(int id);
		void uninstall(bool dialogValue);
	signals:
		void lockChanged();

	private:
		virtual ~GameManager() {};
		GameManager();
		//public slots:

		//signals:
		bool lock_ = false;
		int Gameid_;
		GameUninstaller* uninstaller_;
	};
}

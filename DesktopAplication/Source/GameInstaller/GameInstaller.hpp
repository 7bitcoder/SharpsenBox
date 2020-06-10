#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"

namespace gi {
	class GameInstaller : public bc::IQmlObject {
		Q_OBJECT
	public:
		static GameInstaller& getObject() {
			static GameInstaller uc;
			return uc;
		}

		Q_INVOKABLE void installGame(int id, QString path, bool shortcut);
		Q_INVOKABLE bool getLock() { return lock_; }
		Q_PROPERTY(int lock READ getLock NOTIFY lockChanged);


		// implementation IQmlObject
		void update() override {};
		std::string getName() override;
		void init() override {};
		void lock() { lock_ = true; lockChanged(); }
		void unLock() { lock_ = false; lockChanged(); }

	signals:
		void lockChanged();

	private:
		virtual ~GameInstaller() {};
		GameInstaller() {};
	//public slots:

	//signals:
		bool lock_ = false;
	};
}
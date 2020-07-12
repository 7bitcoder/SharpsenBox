#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "InstalationManager.hpp"
#include <filesystem>

namespace lb {
	class LoadingBar : public bc::IQmlComponent<LoadingBar> {
		Q_OBJECT
	public:
		enum  State : int {
			NONE = -1, CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET
		};
		enum  VisibleState : int {
			HIDDEN = 0, SHOWED, MINIMALIZED
		};

		virtual ~LoadingBar();
		LoadingBar() {};

		// IQmlObject implementation
		void update() override;
		std::string getName() override;
		void init() override;

		void reset();
		//QML Propetries
		Q_PROPERTY(double speed READ getSpeed); //download speed B/s
		Q_PROPERTY(double actual READ getActual); //actual downloaded MB
		Q_PROPERTY(double total READ getTotal); // total MB to download
		Q_PROPERTY(double progress READ getProgress); // total MB to download

		Q_PROPERTY(int state READ getState NOTIFY stateChanged);
		Q_PROPERTY(int visibleState READ getVisibleState NOTIFY visibleStateChanged);

		Q_PROPERTY(QString errorString READ getErrorString NOTIFY errorChanged);

		Q_PROPERTY(int uninstall READ getUninstall);

		//QMl invoklabes
		Q_INVOKABLE double getSpeed() const;
		Q_INVOKABLE double getActual() const;
		Q_INVOKABLE double getTotal() const;
		Q_INVOKABLE double getProgress() const;

		Q_INVOKABLE int getState() const;
		Q_INVOKABLE int getVisibleState() const;

		Q_INVOKABLE int getError() const;
		Q_INVOKABLE QString getErrorString() const;

		Q_INVOKABLE bool getUninstall() const;

		Q_INVOKABLE void pause() const { im::InstalationManager::getObject().pause(); };
		Q_INVOKABLE void resume() const { im::InstalationManager::getObject().resume(); };
		Q_INVOKABLE void stop() const { im::InstalationManager::getObject().stop(); };

	public slots:
		void setTotal(double tot);
		void setActual(double act);
		void setProgress(double prog);
		void setSpeed(double sp);
		void setError(QString& str);
		void setState(State st);
		void setVisibleState(VisibleState st);
		void setUninstallMode(bool un);
	signals:
		void stateChanged();
		void visibleStateChanged();

		void errorChanged();
		void notifyEnded();
		void getProgress(qint64 actual);
	private:
		State state_ = State::NONE;
		VisibleState visibleState_ = VisibleState::HIDDEN;

		//synhronize mutex
		std::mutex mx_;
		//qml properties
		double progress_ = 0;
		double total_ = 0;
		double speed_ = 0;
		double actual_ = 0;
		QString errorStr_ = "";

		bool uninstall_ = false;
	};
}

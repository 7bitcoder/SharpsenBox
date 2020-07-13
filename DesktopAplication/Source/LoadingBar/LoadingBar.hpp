#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include <filesystem>
#include "LoadingBarStates.hpp"
#include "ILoadingBar.hpp"

namespace lb {
	class LoadingBar final :public ILoadingBar {
		Q_OBJECT
	public:


		virtual ~LoadingBar();
		LoadingBar() {};

		// IQmlObject implementation
		void update() final;
		std::string getName() final;
		void init() final;

		void reset() final;
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
		Q_INVOKABLE double getSpeed() const final;
		Q_INVOKABLE double getActual() const final;
		Q_INVOKABLE double getTotal() const final;
		Q_INVOKABLE double getProgress() const final;

		Q_INVOKABLE int getState() const final;
		Q_INVOKABLE int getVisibleState() const final;

		Q_INVOKABLE QString getErrorString() const final;

		Q_INVOKABLE bool getUninstall() const final;

		Q_INVOKABLE void pause() const final;
		Q_INVOKABLE void resume() const final;
		Q_INVOKABLE void stop() const final;

	public slots:
		void setTotal(double tot) final;
		void setActual(double act) final;
		void setProgress(double prog) final;
		void setSpeed(double sp) final;
		void setError(const QString& str) final;
		void setState(State st) final;
		void setVisibleState(VisibleState st) final;
		void setUninstallMode(bool un) final;
	signals:
		void stateChanged() final;
		void visibleStateChanged() final;

		void errorChanged() final;
		void notifyEnded() final;
		void getProgress(qint64 actual) final;
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

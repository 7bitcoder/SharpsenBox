#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include <filesystem>

namespace lb {
	class LoadingBar : public bc::IQmlObject {
		Q_OBJECT
	public:
		static LoadingBar& getObject() {
			static LoadingBar uc;
			return uc;
		}
		enum  State : int {
			CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET
		};
		enum  VisibleState : int {
			HIDDEN = 0, SHOWED, MINIMALIZED
		};

		//!!!!! disconnect(receiver, SLOT(slot()));@ !! disconnect

		// implementation IQmlObject
		void update() override;
		std::string getName() override;
		void init() override;

		void SetTotal(double tot) {
			total_ = tot;
		}
		void SetActual(double act) {
			actual_ = act;
		}
		void SetProgress(double prog) {
			progress_ = prog;
		}
	protected:
		//QML Propetries
		Q_PROPERTY(double speed READ getSpeed); //download speed B/s
		Q_PROPERTY(double actual READ getActual); //actual downloaded MB
		Q_PROPERTY(double total READ getTotal); // total MB to download
		Q_PROPERTY(double progress READ getProgress); // total MB to download

		Q_PROPERTY(int state READ getState NOTIFY stateChanged);
		Q_PROPERTY(int visibleState READ getVisibleState NOTIFY visibleStateChanged);

		Q_PROPERTY(int error READ getError  NOTIFY errorChanged);
		Q_PROPERTY(QString errorString READ getErrorString);

		//QMl invoklabes
		Q_INVOKABLE double getSpeed() const;
		Q_INVOKABLE double getActual() const;
		Q_INVOKABLE double getTotal() const;
		Q_INVOKABLE double getProgress() const;

		Q_INVOKABLE int getState() const;
		Q_INVOKABLE int getVisibleState() const;

		Q_INVOKABLE int getError() const;
		Q_INVOKABLE QString getErrorString() const;


	private:
		virtual ~LoadingBar();
		LoadingBar() {};
	public slots:
		void pauseD();
		void resumeD();
		void stopD();
	signals:
		void stateChanged();
		void visibleStateChanged();

		void errorChanged();
		void notifyEnded();
		void getProgress(qint64 actual);

		void pauseS();
		void resumeS();
		void stopS();

	private:
		State state_ = State::DOWNLOADING;
		VisibleState visibleState_ = VisibleState::HIDDEN;

		//qml properties
		double progress_ = 0;
		double total_ = 0;
		double speed_ = 0;
		double actual_ = 0;
		int error_ = 0;
		QString errorStr_ = "";
	};
}
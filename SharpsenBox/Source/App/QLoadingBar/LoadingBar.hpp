#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include <filesystem>
#include "ILoadingBar.hpp"
#include "IUpdateManager.hpp"

namespace sb {
	class LoadingBar final : public ILoadingBar {
		Q_OBJECT
	public:


		virtual ~LoadingBar();
		LoadingBar();

		// IQmlObject implementation
		void Update() final;
		std::string GetName() final;
		void Init() final;

		void SetTotal(double tot) final;
		void SetActual(double act) final;
		void SetProgress(double prog) final;
		void SetSpeed(double sp) final;
		void SetError(const QString& str) final;
		void SetState(IUpdateManager::State st) final;
		void SetVisibleState(IUpdateManager::VisibleState st) final;
		void SetUninstallMode(bool un) final;
		void Reset() final;
		void Paused() final;
		void Resumed() final;
		//QML Propetries
		Q_PROPERTY(double speed READ GetSpeed); //download speed B/s
		Q_PROPERTY(double actual READ GetActual); //actual downloaded MB
		Q_PROPERTY(double total READ GetTotal); // total MB to download
		Q_PROPERTY(double progress READ GetProgress); // total MB to download

		Q_PROPERTY(int state READ GetState NOTIFY StateChanged);
		Q_PROPERTY(int visibleState READ GetVisibleState NOTIFY VisibleStateChanged);

		Q_PROPERTY(int uninstall READ GetUninstall);
		Q_PROPERTY(bool pauseResume READ GetPauseResume NOTIFY PauseResumeChanged);

		//QMl invoklabes
		Q_INVOKABLE double GetSpeed() ;
		Q_INVOKABLE double GetActual() ;
		Q_INVOKABLE double GetTotal() ;
		Q_INVOKABLE double GetProgress() ;

		Q_INVOKABLE int GetState() ;
		Q_INVOKABLE int GetVisibleState() ;

		Q_INVOKABLE bool GetUninstall() ;

		Q_INVOKABLE void Pause() ;
		Q_INVOKABLE void Resume() ;
		Q_INVOKABLE void Stop() ;
		Q_INVOKABLE bool GetPauseResume();


	signals:
		void StateChanged();
		void VisibleStateChanged();

		void NotifyEnded();
		void GetProgress(qint64 actual);
		void PauseResumeChanged();
	private:
		IUpdateManager::State _State = IUpdateManager::State::NONE;
		IUpdateManager::VisibleState _VisibleState = IUpdateManager::VisibleState::HIDDEN;

		IUpdateManager::State _LastState = IUpdateManager::State::NONE; // when pausing hold last state in this variable for resume

		//qml properties
		double _Progress = 0;
		double _Total = 0;
		double _Speed = 0;
		double _Actual = 0;

		bool _PauseResume = false;
		bool _Uninstall = false;
	};
}

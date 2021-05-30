#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "IComponent.hpp"
#include "IDialog.hpp"
#include "IConfig.hpp"
#include "IGameManager.hpp"

namespace sb
{
	LoadingBar::LoadingBar() {}
	LoadingBar::~LoadingBar() {}

	void LoadingBar::Update() {}
	Q_INVOKABLE std::string LoadingBar::GetName()
	{
		return TYPENAME(LoadingBar);
	}

	Q_INVOKABLE double LoadingBar::GetSpeed()
	{
		return _Speed;
	}

	Q_INVOKABLE int LoadingBar::GetState()
	{
		return static_cast<int>(_State);
	}

	Q_INVOKABLE int LoadingBar::GetVisibleState()
	{

		return static_cast<int>(_VisibleState);
	}

	Q_INVOKABLE double LoadingBar::GetActual()
	{
		return _Actual;
	}

	Q_INVOKABLE double LoadingBar::GetTotal()
	{
		return _Total;
	}

	Q_INVOKABLE double LoadingBar::GetProgress()
	{
		return _Progress;
	}

	Q_INVOKABLE void LoadingBar::Pause()
	{
		Component<IGameManager>::Get().Pause();
	};

	Q_INVOKABLE void LoadingBar::Resume()
	{
		Component<IGameManager>::Get().Resume();
	}

	Q_INVOKABLE void LoadingBar::Stop()
	{
		Component<IGameManager>::Get().Stop();
	}

	Q_INVOKABLE bool LoadingBar::GetPauseResume()
	{
		return _PauseResume;
	}

	Q_INVOKABLE bool LoadingBar::GetUninstall()
	{
		return _Uninstall;
	}

	void LoadingBar::SetTotal(double tot)
	{
		_Total = tot;
	}

	void LoadingBar::SetActual(double act)
	{
		_Actual = act;
	}

	void LoadingBar::SetProgress(double prog)
	{
		_Progress = prog;
	}

	void LoadingBar::SetSpeed(double sp)
	{
		_Speed = sp;
	}

	void LoadingBar::SetError(const QString &str)
	{
		auto &dialog = Component<IDialog>::Get();
		dialog.setType(IDialog::Type::INFO);
		dialog.setInfo(str);
		dialog.show();
	}

	void LoadingBar::SetState(IUpdateManager::State st)
	{
		_State = st;
		StateChanged();
	}

	void LoadingBar::SetVisibleState(IUpdateManager::VisibleState st)
	{
		_VisibleState = st;
		VisibleStateChanged();
	}

	void LoadingBar::SetUninstallMode(bool un)
	{
		_Uninstall = un;
	}

	void LoadingBar::Paused()
	{
		_LastState = _State;
		SetState(IUpdateManager::State::PAUSE);
		_PauseResume = true;
		PauseResumeChanged();
	}

	void LoadingBar::Resumed()
	{
		if (_LastState != IUpdateManager::State::NONE)
			SetState(_LastState);
		_PauseResume = false;
		PauseResumeChanged();
	}

	void LoadingBar::Init() {}

	void LoadingBar::Reset()
	{

		_Progress = 0;
		_Total = 0;
		_Speed = 0;
		_Actual = 0;
		_Uninstall = false;
		_PauseResume = false;

		SetState(IUpdateManager::State::NONE);
		_LastState = IUpdateManager::State::NONE;
		Resumed(); // to Reset pause resume button to pause state
		_VisibleState = IUpdateManager::VisibleState::HIDDEN;
	}
}

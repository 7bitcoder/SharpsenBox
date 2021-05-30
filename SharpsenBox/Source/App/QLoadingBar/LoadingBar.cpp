#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "IComponent.hpp"
#include "IDialog.hpp"
#include "IConfig.hpp"
#include "IGameManager.hpp"


namespace sb {
	LoadingBar::LoadingBar() {}
	LoadingBar::~LoadingBar() {}

	void LoadingBar::Update() {}
	Q_INVOKABLE std::string LoadingBar::GetName() {
		return TYPENAME(LoadingBar);
	}

	Q_INVOKABLE double LoadingBar::getSpeed()  {
		return speed_;
	}

	Q_INVOKABLE int LoadingBar::getState()  {
		return static_cast<int>(state_);
	}

	Q_INVOKABLE int LoadingBar::getVisibleState()  {

		return static_cast<int>(visibleState_);
	}

	Q_INVOKABLE double LoadingBar::getActual()  {
		return actual_;
	}

	Q_INVOKABLE double LoadingBar::getTotal()  {
		return total_;
	}

	Q_INVOKABLE double LoadingBar::getProgress()  {
		return progress_;
	}

	Q_INVOKABLE void LoadingBar::pause()  { Component<IGameManager>::Get().pause(); };
	Q_INVOKABLE void LoadingBar::resume()  { Component<IGameManager>::Get().resume(); }
	Q_INVOKABLE void LoadingBar::stop()  { Component<IGameManager>::Get().stop(); }

	Q_INVOKABLE bool LoadingBar::getUninstall()  {
		return uninstall_;
	}

	void LoadingBar::setTotal(double tot) {
		total_ = tot;
	}

	void LoadingBar::setActual(double act) {
		actual_ = act;
	}

	void LoadingBar::setProgress(double prog) {
		progress_ = prog;
	}

	void LoadingBar::setSpeed(double sp) {
		speed_ = sp;
	}

	void LoadingBar::setError(const QString& str) {
		auto& dialog = Component<IDialog>::Get();
		dialog.setType(IDialog::Type::INFO);
		dialog.setInfo(str);
		dialog.show();
	}

	void LoadingBar::setState(IUpdateManager::State st) {
		state_ = st;
		stateChanged();
	}

	void LoadingBar::setVisibleState(IUpdateManager::VisibleState st) {
		visibleState_ = st;
		visibleStateChanged();
	}

	void LoadingBar::setUninstallMode(bool un) {
		uninstall_ = un;
	}

	void LoadingBar::paused() {
		lastState_ = state_;
		setState(IUpdateManager::State::PAUSE);
		pauseResume_ = true;
		pauseResumeChanged();
	}

	void LoadingBar::resumed() {
		if(lastState_ != IUpdateManager::State::NONE)
			setState(lastState_);
		pauseResume_ = false;
		pauseResumeChanged();
	}
	
	void LoadingBar::Init() {}

	void LoadingBar::Reset() {

		progress_ = 0;
		total_ = 0;
		speed_ = 0;
		actual_ = 0;
		uninstall_ = false;
		pauseResume_ = false;
	
		setState(IUpdateManager::State::NONE);
		lastState_ = IUpdateManager::State::NONE;
		resumed(); // to Reset pause resume button to pause state
		visibleState_ = IUpdateManager::VisibleState::HIDDEN;
	}
}

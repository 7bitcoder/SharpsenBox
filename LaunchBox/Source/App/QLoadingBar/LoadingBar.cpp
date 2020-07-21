#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "IComponent.hpp"
#include "IDialog.hpp"
#include "IConfig.hpp"
#include "IGameManager.hpp"


namespace lb {
	LoadingBar::LoadingBar() {}
	LoadingBar::~LoadingBar() {}

	void LoadingBar::update() {}
	Q_INVOKABLE std::string LoadingBar::getName() {
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

	Q_INVOKABLE void LoadingBar::pause()  { bc::Component<gm::IGameManager>::get().pause(); };
	Q_INVOKABLE void LoadingBar::resume()  { bc::Component<gm::IGameManager>::get().resume(); }
	Q_INVOKABLE void LoadingBar::stop()  { bc::Component<gm::IGameManager>::get().stop(); }

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
		auto& dialog = bc::Component<dl::IDialog>::get();
		dialog.setType(dl::IDialog::Type::INFO);
		dialog.setInfo(str);
		dialog.show();
	}

	void LoadingBar::setState(im::IUpdateManager::State st) {
		state_ = st;
		stateChanged();
	}

	void LoadingBar::setVisibleState(im::IUpdateManager::VisibleState st) {
		visibleState_ = st;
		visibleStateChanged();
	}

	void LoadingBar::setUninstallMode(bool un) {
		uninstall_ = un;
	}

	void LoadingBar::paused() {
		lastState_ = state_;
		setState(im::IUpdateManager::State::PAUSE);
		pauseResume_ = true;
		pauseResumeChanged();
	}

	void LoadingBar::resumed() {
		if(lastState_ != im::IUpdateManager::State::NONE)
			setState(lastState_);
		pauseResume_ = false;
		pauseResumeChanged();
	}
	
	void LoadingBar::init() {}

	void LoadingBar::reset() {

		progress_ = 0;
		total_ = 0;
		speed_ = 0;
		actual_ = 0;
		uninstall_ = false;
		pauseResume_ = false;
	
		setState(im::IUpdateManager::State::NONE);
		lastState_ = im::IUpdateManager::State::NONE;
		resumed(); // to reset pause resume button to pause state
		visibleState_ = im::IUpdateManager::VisibleState::HIDDEN;
	}
}

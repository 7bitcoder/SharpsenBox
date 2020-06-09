#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"

namespace lb {
	LoadingBar::~LoadingBar() {
	}

	void LoadingBar::update() {}
	Q_INVOKABLE std::string LoadingBar::getName() {
		return TYPENAME(LoadingBar);
	}
	Q_INVOKABLE double LoadingBar::getSpeed() const {
		return speed_;
	}
	Q_INVOKABLE int LoadingBar::getState() const {
		return state_;
	}
	Q_INVOKABLE int LoadingBar::getVisibleState() const {
		return visibleState_;
	}
	Q_INVOKABLE double LoadingBar::getActual() const {
		return actual_;
	}
	Q_INVOKABLE double LoadingBar::getTotal() const {
		return total_;
	}
	Q_INVOKABLE double LoadingBar::getProgress() const {
		return progress_;
	}

	Q_INVOKABLE int LoadingBar::getError() const {
		return error_;
	}

	Q_INVOKABLE QString LoadingBar::getErrorString() const {
		return errorStr_;
	}

	void LoadingBar::pauseD() { 
		pauseS(); 
	}
	void LoadingBar::resumeD() { 
		resumeS(); 
	}
	void LoadingBar::stopD() {
		stopS();
	}

	void LoadingBar::init() {
	}
}
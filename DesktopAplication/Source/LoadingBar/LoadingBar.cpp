#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "AppBackend.hpp"
#include "Config.hpp"
#include "InstalationManager.hpp"

namespace lb {
	LoadingBar::~LoadingBar() {}

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

	Q_INVOKABLE void LoadingBar::pause() const { im::InstalationManager::getObject().pause(); };
	Q_INVOKABLE void LoadingBar::resume() const { im::InstalationManager::getObject().resume(); };
	Q_INVOKABLE void LoadingBar::stop() const { im::InstalationManager::getObject().stop(); };

	Q_INVOKABLE QString LoadingBar::getErrorString() const {
		return errorStr_;
	}

	Q_INVOKABLE bool LoadingBar::getUninstall() const {
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
		errorStr_ = str;
		errorChanged();
	}

	void LoadingBar::setState(State st) {
		state_ = st;
		stateChanged();
	}

	void LoadingBar::setVisibleState(VisibleState st) {
		visibleState_ = st;
		visibleStateChanged();
	}

	void LoadingBar::setUninstallMode(bool un) {
		uninstall_ = un;
	}

	void LoadingBar::init() {
		auto* im = &im::InstalationManager::getObject();
		connect(im, im::InstalationManager::setTotalLb, this, LoadingBar::setTotal);
		connect(im, im::InstalationManager::setActualLb, this, LoadingBar::setActual);
		connect(im, im::InstalationManager::setProgressLb, this, LoadingBar::setProgress);
		connect(im, im::InstalationManager::setSpeedLb, this, LoadingBar::setSpeed);
		connect(im, im::InstalationManager::errorEmit, this, LoadingBar::setError);
		connect(im, im::InstalationManager::setStateLb, this, LoadingBar::setState);
		connect(im, im::InstalationManager::setVisibleStateLb, this, LoadingBar::setVisibleState);
		connect(im, im::InstalationManager::setUninstallModeLb, this, LoadingBar::setUninstallMode);
	}

	void LoadingBar::reset() {

		//qml properties
		progress_ = 0;
		total_ = 0;
		speed_ = 0;
		actual_ = 0;
		errorStr_.clear();

		uninstall_ = false;
	}
}

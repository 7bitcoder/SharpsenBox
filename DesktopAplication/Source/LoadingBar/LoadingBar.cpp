#include <iostream>
#include <string>
#include "LoadingBar.hpp"
#include "AppBackend.hpp"
#include "IConfig.hpp"
#include "IInstalationManager.hpp"

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

	Q_INVOKABLE void LoadingBar::pause() const {} //bc::ObjectsRepository::getRepo().getInstalationManager().pause();};
	Q_INVOKABLE void LoadingBar::resume() const {}// bc::ObjectsRepository::getRepo().getInstalationManager().resume(); };
	Q_INVOKABLE void LoadingBar::stop() const {}//bc::ObjectsRepository::getRepo().getInstalationManager().stop(); };

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
		auto* ob = &bc::Backend::getBackend().getInstalationManager();
		//connect(ob, &im::IInstalationManager::setTotalLb, this, &LoadingBar::setTotal);
		//connect(ob, &im::IInstalationManager::setActualLb, this, &LoadingBar::setActual);
		//connect(ob, &im::IInstalationManager::updateProgress, this, &LoadingBar::setProgress);
		//connect(ob, &im::IInstalationManager::setSpeedLb, this, &LoadingBar::setSpeed);
		//connect(ob, &im::IInstalationManager::errorEmit, this, &LoadingBar::setError);
		//connect(ob, &im::IInstalationManager::setStateLb, this, &LoadingBar::setState);
		//connect(ob, &im::IInstalationManager::setVisibleStateLb, this, &LoadingBar::setVisibleState);
		//connect(ob, &im::IInstalationManager::setUninstallModeLb, this, &LoadingBar::setUninstallMode);
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

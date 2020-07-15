#include <iostream>
#include <string>
#include "GameManager.hpp"
#include "IComponent.hpp"
#include "IConfig.hpp"
#include "Game.hpp"
#include "GameUninstaller.hpp"
#include "ILoadingBar.hpp"
#include "IDialog.hpp"


namespace gm {

	std::string GameManager::getName() {
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : uninstaller_(new GameUninstaller) {};


	Q_INVOKABLE void GameManager::installGame(int id, QString path, bool shortcut) {
		if (!lock_) {
			lock();
			auto& game = bc::Component <cf::IConfig>::get().getGame(id);
			std::string gg = path.toStdString();
#ifdef _WIN32
			if (gg.starts_with("/"))
				gg = gg.substr(1);
#endif 
			std::filesystem::path p = gg;
			p /= game.name.toStdString();
			game.gameDir = p.generic_string().c_str();
			game.shortcut = shortcut;
			im_.updateGame(game);
		}
	}
	void GameManager::init() {
		im_.init();
		lb_ = &bc::Component<lb::ILoadingBar>::get();
		connect(uninstaller_, &GameUninstaller::uninstalationComplete, this, &GameManager::uninstallation);

		connect(&im_, &im::UpdateManager::errorEmit, this, &GameManager::errorEmit);
		connect(&im_, &im::UpdateManager::updateProgress, this, &GameManager::updateProgress);
		connect(&im_, &im::UpdateManager::setTotalLb, this, &GameManager::setTotalLb);
		connect(&im_, &im::UpdateManager::setActualLb, this, &GameManager::setActualLb);
		connect(&im_, &im::UpdateManager::setSpeedLb, this, &GameManager::setSpeedLb);
		connect(&im_, &im::UpdateManager::setStateLb, this, &GameManager::setStateLb);
		connect(&im_, &im::UpdateManager::setVisibleStateLb, this, &GameManager::setVisibleStateLb);
		connect(&im_, &im::UpdateManager::setUninstallModeLb, this, &GameManager::setUninstallModeLb);
	};

	Q_INVOKABLE void GameManager::unistallRequest(int id) {
		Gameid_ = id;
		auto& dialog = bc::Component < dl::IDialog > ::get();
		dialog.setInfo(QString("Are you sure you want to remove ") + bc::Component <cf::IConfig>::get().getGameName(id));
		dialog.show();
	}

	void GameManager::uninstall(bool dialogValue) {
		if (dialogValue) {
			lock();
			auto& lb = bc::Component < lb::ILoadingBar > ::get();
			lb.reset();
			//lb.setUninstallMode(true);
			//lb.setState(lb::State::CHECKING);
			//lb.setVisibleState(lb::VisibleState::SHOWED);
			uninstaller_->setId(Gameid_);
			uninstaller_->start();
		} else {} //nothing
		auto& dialog = bc::Component < dl::IDialog > ::get();
	}

	void GameManager::uninstallation(int id) {
		bc::Component <cf::IConfig>::get().getGame(id).installed = false;
		unLock();
		auto& lb = bc::Component < lb::ILoadingBar > ::get();
		lb.reset();
		//lb.setUninstallMode(false);
		//lb.setState(lb::State::COMPLEET);
		//lb.setVisibleState(lb::VisibleState::HIDDEN);
	}

	Q_INVOKABLE void GameManager::checkAutoUpdate(int id) {
		auto& game = bc::Component <cf::IConfig>::get().getGame(id);
		if (game.autoCheck && game.installed) {
			update(id);
		}
	}

	Q_INVOKABLE void GameManager::update(int id) {
		auto& game = bc::Component <cf::IConfig>::get().getGame(id);
		if (game.updateChecked)
			return; //checked
	}

	Q_INVOKABLE void GameManager::runGame(int id) {
		auto& game = bc::Component <cf::IConfig>::get().getGame(id);
		std::filesystem::path path = game.gameDir.toUtf8().constData();
		path /= game.execPath.toUtf8().constData();
		std::string cd = "cd \"";
		cd += path.parent_path().generic_string() + "\"";
		std::string execute = path.filename().generic_string();
		std::string total = cd + " && " + execute;
		system(total.c_str());
	}

	void GameManager::errorEmit(const QString& errorStr) { lb_->setError(errorStr); }
	void GameManager::updateProgress(double prog) { lb_->setProgress(prog); }

	void GameManager::setTotalLb(double tot) { lb_->setTotal(tot); }
	void GameManager::setActualLb(double act) { lb_->setActual(act); }
	void GameManager::setSpeedLb(double sp) { lb_->setSpeed(sp); }
	void GameManager::setStateLb(im::State st) { lb_->setState(st); }
	void GameManager::setVisibleStateLb(im::VisibleState st) { lb_->setVisibleState(st); }
	void GameManager::setUninstallModeLb(bool un) { lb_->setUninstallMode(un); }
}

#include <iostream>
#include <string>
#include <functional>
#include "GameManager.hpp"
#include "IComponent.hpp"
#include "IConfig.hpp"
#include "Game.hpp"
#include "GameUninstaller.hpp"
#include "IUpdateManager.hpp"
#include "ILoadingBar.hpp"
#include "IDialog.hpp"
#include "updateInfo.hpp"


namespace gm {

	std::string GameManager::getName() {
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : uninstaller_(new GameUninstaller) {};


	Q_INVOKABLE void GameManager::installGameRequest(int id) {
		if (checkProcess())
			return;
		lock();
		auto callback = [id, this](bool val) {
			if (val) {
				auto& game = bc::Component <cf::IConfig>::get().getGame(id);
				std::string gg = path_.toStdString();
#ifdef _WIN32
				if (gg.starts_with("/"))
					gg = gg.substr(1);
#endif 
				std::filesystem::path p = gg;
				p /= game.name.toStdString();
				bc::Component <lb::ILoadingBar>::get().reset();
				im_.installGame(game, p.generic_string().c_str(), shortcut_);
			} else {
				unLock();
			}
			return false;
		};
		auto& dialog = bc::Component<dl::IDialog>::get();
		dialog.setType(dl::IDialog::INSTALL);
		dialog.setCallback(callback);
		dialog.setInfo("");
		dialog.show();
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
		connect(&im_, &im::UpdateManager::gameUpdateEnded, this, &GameManager::gameUpdateEnded);
	};

	Q_INVOKABLE void GameManager::unistallRequest(int id) {
		gameId_ = id;
		if (checkProcess())
			return;
		auto& cf = bc::Component <cf::IConfig>::get();
		auto& dialog = bc::Component < dl::IDialog > ::get();
		if (cf.installed(id)) {
			dialog.setType(dl::IDialog::DIALOG);
			dialog.setCallback([this](bool val) {return this->uninstall(val); });
			dialog.setInfo(QString("Are you sure you want to remove ") + cf.getGameName(id));
			dialog.show();
		} else {
			dialog.setType(dl::IDialog::INFO);
			dialog.setInfo("Game is not installed");
			dialog.show();
		}
	}

	bool GameManager::uninstall(bool dialogValue) {
		if (dialogValue) {
			if (checkProcess())
				return false;
			lock();
			lb_->reset();
			lb_->setUninstallMode(true);
			lb_->setState(im::IUpdateManager::State::CHECKING);
			lb_->setVisibleState(im::IUpdateManager::VisibleState::SHOWED);
			uninstaller_->setId(gameId_);
			uninstaller_->start();
			return false;
		} else {} //nothing
		return false;
	}

	void GameManager::uninstallation(int id) {
		bc::Component <cf::IConfig>::get().getGame(id).installed = false;
		unLock();
		lb_->reset();
		lb_->setUninstallMode(false);
		lb_->setVisibleState(im::IUpdateManager::VisibleState::HIDDEN);
	}

	Q_INVOKABLE void GameManager::checkAutoUpdate(int id) {
		auto& game = bc::Component <cf::IConfig>::get().getGame(id);
		if (game.autoCheck && game.installed) {
			if (lock_) {
				auto& dialog = bc::Component<dl::IDialog>::get();
				dialog.setType(dl::IDialog::INFO);
				dialog.setInfo(QString("Cannot auto update game ") + game.name + "another process is running");
				dialog.show();
			} else {
				lock();
				update(id);
			}
		}
	}

	Q_INVOKABLE void GameManager::update(int id) {
		auto& game = bc::Component <cf::IConfig>::get().getGame(id);
		if (!game.updateChecked) {
			bc::Component <lb::ILoadingBar>::get().reset();
			im_.updateGame(game);
		}
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

	void GameManager::setUninstallModeLb(bool un) { lb_->setUninstallMode(un); }

	void GameManager::gameUpdateEnded() {
		auto& updatedGame = im_.getUpdateInfo().getActualGame();
		bc::Component<cf::IConfig>::get().getGame(updatedGame.id) = updatedGame; // insert new game info in cofnig
		unLock();
	}
	bool GameManager::checkProcess() {
		if (lock_) {
			auto& dialog = bc::Component<dl::IDialog>::get();
			dialog.setType(dl::IDialog::INFO);
			dialog.setInfo("Wait for other process to finish");
			dialog.show();
			return true;
		} else {
			return false;
		}
	}
}

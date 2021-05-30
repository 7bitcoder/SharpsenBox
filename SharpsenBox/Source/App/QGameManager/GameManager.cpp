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


namespace sb {

	std::string GameManager::GetName() {
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : uninstaller_(new GameUninstaller) {};

	GameManager::~GameManager() {
		UpdateManager.terminate();
		UpdateManager.wait();
		uninstaller_->terminate();
		uninstaller_->wait();
	};

	Q_INVOKABLE void GameManager::installGameRequest(int id) {
		if (checkProcess())
			return;
		lock();
		auto callback = [id, this](bool val) {
			if (val) {
				lb_->Reset();
				auto& game = Component <IConfig>::Get().GetGame(id);
				std::string gg = path_.toStdString();
#ifdef _WIN32
				if (gg.starts_with("/"))
					gg = gg.substr(1);
#endif 
				std::filesystem::path p = gg;
				p /= game.Title.toStdString();
				UpdateManager.installGame(game, p.generic_string().c_str(), shortcut_);
			} else {
				unLock();
			}
			return false;
		};
		auto& dialog = Component<IDialog>::Get();
		dialog.setType(IDialog::INSTALL);
		dialog.setCallback(callback);
		dialog.setInfo("");
		dialog.show();
	}

	void GameManager::Init() {
		UpdateManager.Init();
		lb_ = &Component<ILoadingBar>::Get();
		connect(uninstaller_, &GameUninstaller::uninstalationComplete, this, &GameManager::uninstallation);

		connect(&UpdateManager, &UpdateManager::errorEmit, this, &GameManager::errorEmit);
		connect(&UpdateManager, &UpdateManager::updateProgress, this, &GameManager::updateProgress);
		connect(&UpdateManager, &UpdateManager::setTotalLb, this, &GameManager::setTotalLb);
		connect(&UpdateManager, &UpdateManager::setActualLb, this, &GameManager::setActualLb);
		connect(&UpdateManager, &UpdateManager::setSpeedLb, this, &GameManager::setSpeedLb);
		connect(&UpdateManager, &UpdateManager::setStateLb, this, &GameManager::setStateLb);
		connect(&UpdateManager, &UpdateManager::setVisibleStateLb, this, &GameManager::setVisibleStateLb);
		connect(&UpdateManager, &UpdateManager::setUninstallModeLb, this, &GameManager::setUninstallModeLb);
		connect(&UpdateManager, &UpdateManager::pausedSignal, this, &GameManager::paused);
		connect(&UpdateManager, &UpdateManager::resumedSignal, this, &GameManager::resumed);
	};

	Q_INVOKABLE void GameManager::unistallRequest(int id) {
		gameId_ = id;
		if (checkProcess())
			return;
		auto& cf = Component <IConfig>::Get();
		auto& dialog = Component < IDialog > ::Get();
		if (cf.IsGameInstalled(id)) {
			dialog.setType(IDialog::DIALOG);
			dialog.setCallback([this](bool val) {return this->uninstall(val); });
			dialog.setInfo(QString("Are you sure you want to remove ") + cf.GetGameTitle(id));
			dialog.show();
		} else {
			dialog.setType(IDialog::INFO);
			dialog.setInfo("Game is not installed");
			dialog.show();
		}
	}

	bool GameManager::uninstall(bool dialogValue) {
		if (dialogValue) {
			if (checkProcess())
				return false;
			lock();
			lb_->Reset();
			lb_->SetUninstallMode(true);
			lb_->SetState(IUpdateManager::State::CHECKING);
			lb_->SetVisibleState(IUpdateManager::VisibleState::SHOWED);
			uninstaller_->setId(gameId_);
			uninstaller_->start();
			return false;
		} else {} //nothing
		return false;
	}

	void GameManager::pause() { if(lock_) UpdateManager.pause(); }

	void GameManager::resume() { if (lock_) UpdateManager.resume(); }

	void GameManager::stop() { if (lock_) UpdateManager.stop(); }

	void GameManager::uninstallation(int id) {
		Component <IConfig>::Get().GetGame(id).IsInstalled = false;
		unLock();
		lb_->SetUninstallMode(false);
		lb_->SetVisibleState(IUpdateManager::VisibleState::HIDDEN);
	}

	Q_INVOKABLE void GameManager::checkAutoUpdate(int id) {
		auto& game = Component<IConfig>::Get().GetGame(id);
		if (game.UpdateAutoCheck && game.IsInstalled) {
			if (lock_) {
				auto& dialog = Component<IDialog>::Get();
				dialog.setType(IDialog::INFO);
				dialog.setInfo(QString("Cannot auto Update game ") + game.Title + " another process is running");
				dialog.show();
			} else {
				lock();
				Update(id);
			}
		}
	}

	Q_INVOKABLE void GameManager::Update(int id) {
		auto& game = Component <IConfig>::Get().GetGame(id);
		if (!game.UpdateChecked) {
			Component <ILoadingBar>::Get().Reset();
			UpdateManager.updateGame(game);
		}
	}

	Q_INVOKABLE void GameManager::runGame(int id) {
		auto& game = Component <IConfig>::Get().GetGame(id);
		std::filesystem::path path = game.GameDir.toUtf8().constData();
		path /= game.ExecutablePath.toUtf8().constData();
		std::string cd = "cd \"";
		cd += path.parent_path().generic_string() + "\"";
		std::string execute = path.filename().generic_string();
		std::string total = cd + " && " + execute;
		system(total.c_str());
	}

	void GameManager::errorEmit(const QString& errorStr) { lb_->SetError(errorStr); }
	void GameManager::updateProgress(double prog) { lb_->SetProgress(prog); }

	void GameManager::setTotalLb(double tot) { lb_->SetTotal(tot); }
	void GameManager::setActualLb(double act) { lb_->SetActual(act); }
	void GameManager::setSpeedLb(double sp) { lb_->SetSpeed(sp); }

	void GameManager::setUninstallModeLb(bool un) { lb_->SetUninstallMode(un); }

	void GameManager::setStateLb(int state) { 
		using State = IUpdateManager::State;
		auto st = static_cast<State>(state);
		switch (st) {
		case State::COMPLEET: {
			auto& updatedGame = UpdateManager.getUpdateInfo().getActualGame();
			Component<IConfig>::Get().GetGame(updatedGame.Id) = updatedGame; // insert new game info in cofnig
		}
		case State::ERRORD:
		case State::STOPPED:
			unLock();
			break;
		default:
			break;
		}
		lb_->SetState(st);
	}

	void GameManager::setVisibleStateLb(int st) { 
		lb_->SetVisibleState(IUpdateManager::VisibleState(st)); 
	}

	bool GameManager::checkProcess() {
		if (lock_) {
			auto& dialog = Component<IDialog>::Get();
			dialog.setType(IDialog::INFO);
			dialog.setInfo("Wait for other process to finish");
			dialog.show();
			return true;
		} else {
			return false;
		}
	}
}

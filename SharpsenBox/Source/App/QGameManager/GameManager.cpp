#include "GameManager.hpp"

#include <functional>
#include <iostream>
#include <string>

#include "Game.hpp"
#include "GameUninstaller.hpp"
#include "IComponent.hpp"
#include "IConfig.hpp"
#include "IDialog.hpp"
#include "ILoadingBar.hpp"
#include "IUpdateManager.hpp"
#include "updateInfo.hpp"

namespace sb
{

	std::string
	GameManager::GetName()
	{
		return TYPENAME(GameManager);
	}
	GameManager::GameManager() : _Uninstaller(new GameUninstaller){};

	GameManager::~GameManager()
	{
		_UpdateManager.terminate();
		_UpdateManager.wait();
		_Uninstaller->terminate();
		_Uninstaller->wait();
	};

	Q_INVOKABLE void
	GameManager::InstallGameRequest(int id)
	{
		if (CheckProcess())
			return;
		Lock();
		auto callback = [id, this](bool val)
		{
			if (val)
			{
				_LoadingBar->Reset();
				auto &game = Component<IConfig>::Get().GetGame(id);
				std::string gg = _Path.toStdString();
#ifdef _WIN32
				if (gg.starts_with("/"))
					gg = gg.substr(1);
#endif
				std::filesystem::path p = gg;
				p /= game.Title.toStdString();
				_UpdateManager.installGame(game, p.generic_string().c_str(),
										   _Shortcut);
			}
			else
			{
				UnLock();
			}
			return false;
		};
		auto &dialog = Component<IDialog>::Get();
		dialog.SetType(IDialog::INSTALL);
		dialog.SetCallback(callback);
		dialog.SetInfo("");
		dialog.Show();
	}

	void
	GameManager::Init()
	{
		_UpdateManager.Init();
		_LoadingBar = &Component<ILoadingBar>::Get();
		connect(_Uninstaller, &GameUninstaller::UninstalationComplete, this,
				&GameManager::Uninstallation);

		connect(&_UpdateManager, &UpdateManager::errorEmit, this,
				&GameManager::ErrorEmit);
		connect(&_UpdateManager, &UpdateManager::updateProgress, this,
				&GameManager::UpdateProgress);
		connect(&_UpdateManager, &UpdateManager::setTotalLb, this,
				&GameManager::SetTotalLb);
		connect(&_UpdateManager, &UpdateManager::setActualLb, this,
				&GameManager::SetActualLb);
		connect(&_UpdateManager, &UpdateManager::setSpeedLb, this,
				&GameManager::SetSpeedLb);
		connect(&_UpdateManager, &UpdateManager::setStateLb, this,
				&GameManager::SetStateLb);
		connect(&_UpdateManager, &UpdateManager::setVisibleStateLb, this,
				&GameManager::SetVisibleStateLb);
		connect(&_UpdateManager, &UpdateManager::setUninstallModeLb, this,
				&GameManager::SetUninstallModeLb);
		connect(&_UpdateManager, &UpdateManager::pausedSignal, this,
				&GameManager::Paused);
		connect(&_UpdateManager, &UpdateManager::resumedSignal, this,
				&GameManager::Resumed);
	};

	Q_INVOKABLE void
	GameManager::UnistallRequest(int id)
	{
		_GameId = id;
		if (CheckProcess())
			return;
		auto &cf = Component<IConfig>::Get();
		auto &dialog = Component<IDialog>::Get();
		if (cf.IsGameInstalled(id))
		{
			dialog.SetType(IDialog::DIALOG);
			dialog.SetCallback([this](bool val)
							   { return this->Uninstall(val); });
			dialog.SetInfo(QString("Are you sure you want to remove ") + cf.GetGameTitle(id));
			dialog.Show();
		}
		else
		{
			dialog.SetType(IDialog::INFO);
			dialog.SetInfo("Game is not installed");
			dialog.Show();
		}
	}

	bool
	GameManager::Uninstall(bool dialogValue)
	{
		if (dialogValue)
		{
			if (CheckProcess())
				return false;
			Lock();
			_LoadingBar->Reset();
			_LoadingBar->SetUninstallMode(true);
			_LoadingBar->SetState(IUpdateManager::State::CHECKING);
			_LoadingBar->SetVisibleState(IUpdateManager::VisibleState::SHOWED);
			_Uninstaller->SetGameId(_GameId);
			_Uninstaller->start();
			return false;
		}
		else
		{
		} // nothing
		return false;
	}

	void
	GameManager::Pause()
	{
		if (_Lock)
			_UpdateManager.pause();
	}

	void
	GameManager::Resume()
	{
		if (_Lock)
			_UpdateManager.resume();
	}

	void
	GameManager::Stop()
	{
		if (_Lock)
			_UpdateManager.stop();
	}

	void
	GameManager::Uninstallation(int id)
	{
		Component<IConfig>::Get().GetGame(id).IsInstalled = false;
		UnLock();
		_LoadingBar->SetUninstallMode(false);
		_LoadingBar->SetVisibleState(IUpdateManager::VisibleState::HIDDEN);
	}

	Q_INVOKABLE void
	GameManager::CheckAutoUpdate(int id)
	{
		auto &game = Component<IConfig>::Get().GetGame(id);
		if (game.UpdateAutoCheck && game.IsInstalled)
		{
			if (_Lock)
			{
				auto &dialog = Component<IDialog>::Get();
				dialog.SetType(IDialog::INFO);
				dialog.SetInfo(QString("Cannot auto Update game ") + game.Title + " another process is running");
				dialog.Show();
			}
			else
			{
				Lock();
				Update(id);
			}
		}
	}

	Q_INVOKABLE void
	GameManager::Update(int id)
	{
		auto &game = Component<IConfig>::Get().GetGame(id);
		if (!game.UpdateChecked)
		{
			Component<ILoadingBar>::Get().Reset();
			_UpdateManager.updateGame(game);
		}
	}

	Q_INVOKABLE void
	GameManager::RunGame(int id)
	{
		auto &game = Component<IConfig>::Get().GetGame(id);
		std::filesystem::path path = game.GameDir.toUtf8().constData();
		path /= game.ExecutablePath.toUtf8().constData();
		std::string cd = "cd \"";
		cd += path.parent_path().generic_string() + "\"";
		std::string execute = path.filename().generic_string();
		std::string total = cd + " && " + execute;
		system(total.c_str());
	}

	void
	GameManager::ErrorEmit(const QString &errorStr)
	{
		_LoadingBar->SetError(errorStr);
	}
	void
	GameManager::UpdateProgress(double prog)
	{
		_LoadingBar->SetProgress(prog);
	}

	void
	GameManager::SetTotalLb(double tot)
	{
		_LoadingBar->SetTotal(tot);
	}
	void
	GameManager::SetActualLb(double act)
	{
		_LoadingBar->SetActual(act);
	}
	void
	GameManager::SetSpeedLb(double sp)
	{
		_LoadingBar->SetSpeed(sp);
	}

	void
	GameManager::SetUninstallModeLb(bool un)
	{
		_LoadingBar->SetUninstallMode(un);
	}

	void
	GameManager::SetStateLb(int state)
	{
		using State = IUpdateManager::State;
		auto st = static_cast<State>(state);
		switch (st)
		{
		case State::COMPLEET:
		{
			auto &updatedGame = _UpdateManager.getUpdateInfo().getActualGame();
			Component<IConfig>::Get().GetGame(updatedGame.Id) = updatedGame; // insert new game info in cofnig
		}
		case State::ERRORD:
		case State::STOPPED:
			UnLock();
			break;
		default:
			break;
		}
		_LoadingBar->SetState(st);
	}

	void
	GameManager::SetVisibleStateLb(int st)
	{
		_LoadingBar->SetVisibleState(IUpdateManager::VisibleState(st));
	}

	bool
	GameManager::CheckProcess()
	{
		if (_Lock)
		{
			auto &dialog = Component<IDialog>::Get();
			dialog.SetType(IDialog::INFO);
			dialog.SetInfo("Wait for other process to finish");
			dialog.Show();
			return true;
		}
		else
		{
			return false;
		}
	}
} // namespace sb

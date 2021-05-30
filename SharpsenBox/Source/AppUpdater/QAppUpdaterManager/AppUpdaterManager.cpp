#include "AppUpdaterManager.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "UpdateManager.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <string>

namespace sb
{

	AppUpdaterManager::AppUpdaterManager() : _Config(Component<IConfig>::Get())
	{
		_Config.Init();
		_UpdateManager.Init();
		auto &downloadDir = _Config.GetDownloadDir();
		if (!QDir().exists(downloadDir))
			QDir().mkdir(downloadDir);
	}

	AppUpdaterManager::~AppUpdaterManager()
	{
		_UpdateManager.terminate();
		_UpdateManager.wait();
	}

	void AppUpdaterManager::CheckForUpdates()
	{
		connect(&_UpdateManager, &UpdateManager::setStateLb, this, &AppUpdaterManager::UpdateSt);
		connect(&_UpdateManager, &UpdateManager::errorEmit, this, &AppUpdaterManager::ErrorCatched);
		connect(&_UpdateManager, &UpdateManager::updateEnded, this, &AppUpdaterManager::UpdateInstalled);
		connect(&_UpdateManager, &UpdateManager::updateProgress, this, &AppUpdaterManager::UpdateProgress);
		UpdateStatus(IUpdateManager::State::DOWNLOADING);
		if (_Config.GetVersion() == "0") // needInstallation
			_UpdateManager.installMainApp(_Config.GetVersion(), _Config.GetLauncherAppInfoUrl(), _Config.GetGameInfoRepository());
		else //just Update
			_UpdateManager.updateMainApp(_Config.GetVersion(), _Config.GetLauncherAppInfoUrl(), _Config.GetGameInfoRepository());
	}

	void AppUpdaterManager::UpdateSt(int state)
	{
		UpdateStatus(static_cast<IUpdateManager::State>(state));
	}

	void AppUpdaterManager::UpdateStatus(IUpdateManager::State state)
	{
		_State = state;
		StateChanged();
	}

	void AppUpdaterManager::UpdateInstalled(const QString &version)
	{
		if (!version.isEmpty())
		{
			UpdateStatus(IUpdateManager::State::COMPLEET);
			_Config.SetVersion(version);
		}
	}

	void AppUpdaterManager::ErrorCatched(const QString &what)
	{
		_StatusStr = what;
		UpdateStatus(IUpdateManager::State::ERRORD);
	}

	void AppUpdaterManager::UpdateProgress(double progress)
	{
		_Progress = progress;
		ProgressChanged();
	}

	Q_INVOKABLE QString AppUpdaterManager::GetStateStr() { return _StatusStr; }
	Q_INVOKABLE int AppUpdaterManager::GetProgress() { return _Progress; }
	Q_INVOKABLE int AppUpdaterManager::GetState() { return static_cast<int>(_State); }
}

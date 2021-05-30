#include <QDir>
#include "GameUninstaller.hpp"
#include "IConfig.hpp"
#include "IComponent.hpp"
#include "Game.hpp"

namespace sb
{
	void GameUninstaller::SetGameId(int id)
	{
		_GameId = id;
		auto &game = Component<IConfig>::Get().GetGame(_GameId);
		_GameDir = game.GameDir;
		_Shortcut = game.HasShortcut;
		if (_Shortcut)
			_ShortcutPath = game.ShortcutPath;
		start();
	}

	void GameUninstaller::run()
	{
		try
		{
			if (QDir().exists(_GameDir))
				QDir(_GameDir).removeRecursively();
			if (_Shortcut && QFileInfo::exists(_ShortcutPath))
				QFile(_ShortcutPath).remove();
		}
		catch (...)
		{
		}
		UninstalationComplete(_GameId);
	}

}

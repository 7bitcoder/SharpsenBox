#pragma once
#include <filesystem>
#include <string>
#include <QString>
#include <unordered_map>
namespace sb
{
	struct AppPack
	{
		QString Url;
		QString FileName;
		QString Destination;
	};

	struct Game
	{
		int Id;
		bool IsInstalled;
		bool HasShortcut;
		bool UpdateAutoCheck;

		QString Title;
		QString Version;

		QString GameDir;
		QString ExecutablePath;
		QString ShortcutPath;

		QString GameInfoUrl;
		QString PresentationUrl;
		QString PresentationQml;
		QString PresentationPackUrl;
		QString PresentationVer;

		bool UpdateChecked = false;
		bool operator==(const Game &g)
		{
			return Id == g.Id;
		}
	};
}
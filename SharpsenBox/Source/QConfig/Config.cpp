#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <iostream>
#include <algorithm>
#include <QDir>

namespace sb
{

	namespace
	{
		bool readBool(const QJsonValue &val) { return val.toString() == "1"; }
		QString writeBool(bool f) { return f ? "1" : "0"; }

		QString readJsonFile(const QString &filePath)
		{
			QString val;
			QFile file;
			//open SharpsenBoxConfig file
			file.setFileName(filePath);
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			file.close();
			return val;
		}
	}

	QString Config::CombinePath(const QList<QString> paths)
	{
		if (paths.isEmpty())
			return "";
		QString outputPath = paths.at(0);
		for (int i = 1; i < paths.size(); ++i)
		{
			outputPath + QDir::separator() + paths.at(i);
		}
		return outputPath;
	}

	void Config::Update(){};

	void Config::Init(){};

	std::string Config::GetName()
	{
		return TYPENAME(Config);
	}

	void Config::AddNewGame(Game &game)
	{
		_Games.insert(game.Id, game);
	};

	QString &Config::GetVersion()
	{
		return _Version;
	}
	void Config::SetVersion(QString ver)
	{
		_Version = ver;
	}

	QString &Config::GetDownloadDir()
	{
		return _DownloadDir;
	}

	QString Config::GetConfigJsonFilePath()
	{
		return CombinePath({_Config, _ConfigJson});
	}

	QString Config::GetConfigJsonFileName()
	{
		return _ConfigJson;
	}

	QString Config::GetLauncherAppInfoUrl()
	{
		return _LauncherAppInfo;
	}

	QString Config::GetGameInfoRepository()
	{
		return _GameInfoRepo;
	}

	Config::Config()
	{
		try
		{
			if (!QDir(_Config).exists())
				QDir().mkdir(_Config);
			if (!QFileInfo::exists(GetConfigJsonFilePath()))
				; //problem
			QString val = readJsonFile(GetConfigJsonFilePath());
			auto ff = val.toStdString();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

			// Read settings
			_Version = d["Ver"].toString();
			_DownloadSpeed = d["DownloadSpeed"].toString().toInt();
			_GameInfoRepo = d["GamesInfoRepository"].toString();
			_LauncherAppInfo = d["AppInfoUrl"].toString();

			std::cout << "version: " << _Version.toStdString();
			ReadGames();

			for (auto &key : _Games.keys())
			{
				_SortedId.push_back(key);
			}
			std::sort(_SortedId.begin(), _SortedId.end(), std::less<int>());
		}
		catch (std::exception &e)
		{
			throw; // todo use issue logger
		}
		catch (...)
		{
			throw;
		}
	}

	void Config::ReadGames()
	{

		QString val = readJsonFile(CombinePath({_Config, _GamesFileName}));

		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject json = d.object();
		for (auto &key : json.keys())
		{
			QJsonObject value = json[key].toObject();
			auto game = ReadGameInfo(value);
			game.Title = key;
			_Games.insert(game.Id, game);
		}
	}
	Game Config::ReadGameInfo(const QJsonObject &value)
	{
		Game g;
		g.Id = value["Id"].toString().toInt();
		g.IsInstalled = readBool(value["Installed"]);
		g.HasShortcut = readBool(value["Shortcut"]);
		g.UpdateAutoCheck = readBool(value["AutoUpdate"]);
		g.Version = value["Ver"].toString();
		g.GameInfoUrl = value["AppInfoUrl"].toString();
		g.GameDir = value["GameDir"].toString();
		g.ExecutablePath = value["GameExecPath"].toString();
		g.ShortcutPath = value["ShortcutPath"].toString();
		g.PresentationUrl = value["PresentationUrl"].toString();
		g.PresentationQml = value["PresentationQml"].toString();
		g.PresentationPackUrl = value["PresentationPackUrl"].toString();
		g.PresentationVer = value["PresentationVer"].toString();
		//std::cout << g.id << g.url.toUtf8().constData() << g.version.toUtf8().constData() << g.IsInstalled;
		return g;
	}

	void Config::WriteGames()
	{
		QJsonObject RootObject;
		for (auto &game : _Games)
		{
			auto jsonParsed = ParseGameInfo(game);
			RootObject.insert(game.Title, jsonParsed);
		}
		QFile file;
		QJsonDocument d;
		d.setObject(RootObject);
		file.setFileName(CombinePath({_Config, _GamesFileName}));
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();
	}

	QJsonObject Config::ParseGameInfo(const Game &game)
	{
		QJsonObject RootObject;
		RootObject.insert("Id", QString::number(game.Id));
		RootObject.insert("Ver", game.Version);
		RootObject.insert("AutoUpdate", writeBool(game.UpdateAutoCheck));
		RootObject.insert("IsInstalled", writeBool(game.IsInstalled));
		RootObject.insert("Shortcut", writeBool(game.HasShortcut));
		RootObject.insert("GameDir", game.GameDir);
		RootObject.insert("GameExecPath", game.ExecutablePath);
		RootObject.insert("ShortcutPath", game.ShortcutPath);
		RootObject.insert("PresentationUrl", game.PresentationUrl);

		RootObject.insert("PresentationQml", game.PresentationQml);
		RootObject.insert("PresentationPackUrl", game.PresentationPackUrl);
		RootObject.insert("PresentationVer", game.PresentationVer);
		RootObject.insert("AppInfoUrl", game.GameInfoUrl);
		return RootObject;
	}

	Config::~Config()
	{
		QFile file;

		QJsonDocument d;
		QJsonObject RootObject = d.object();

		RootObject.insert("Ver", _Version);
		RootObject.insert("DownloadSpeed", QString::number(_DownloadSpeed));
		RootObject.insert("GamesInfoRepository", _GameInfoRepo);
		RootObject.insert("AppInfoUrl", _LauncherAppInfo);

		d.setObject(RootObject);
		file.setFileName(GetConfigJsonFilePath());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();

		WriteGames();
	}
	//
	//qint64 AppUpdateChecker::getProgress() const {
	//	return progress_;
	//}

	Q_INVOKABLE bool Config::IsGameInstalled(int id)
	{
		auto it = _Games.find(id);
		if (it != _Games.end())
			return it->IsInstalled;
		return false;
	}

	Q_INVOKABLE QUrl Config::DefaultInstallDir()
	{
		if (getenv("PROGRAMFILES"))
		{ //windows
			std::filesystem::path path = getenv("PROGRAMFILES");
			return QUrl::fromLocalFile(path.generic_string().c_str());
		}
		else
		{ //mac /linux
			return QString();
		}
		return QString();
	}

	Q_INVOKABLE void Config::SetDownloadSpeed(qint32 dp)
	{
		_DownloadSpeed = dp;
	}

	Q_INVOKABLE qint32 Config::GetDownloadSpeed()
	{
		return _DownloadSpeed;
	}

	Q_INVOKABLE void Config::SetGameUpdateAutoCheck(int id, bool val)
	{
		GetGame(id).UpdateAutoCheck = val;
	}

	Q_INVOKABLE bool Config::GetGameUpdateAutoCheck(int id)
	{
		bool v = GetGame(id).UpdateAutoCheck;
		return v;
	}

	Game &Config::GetGame(int id)
	{
		if (_Games.contains(id))
		{
			return _Games[id];
		}
		throw std::exception("Bad game Id");
	}

	bool Config::GameExists(int id)
	{
		return _Games.contains(id);
	}

	Q_INVOKABLE QString Config::GameInfoDir(int id)
	{
		return CombinePath({GetCurrentDirectory(), "Games", GetGame(id).Title, ""});
	}

	QString Config::GamePageDir(int id)
	{
		return CombinePath({"../Games/", GetGame(id).Title});
	}

	Q_INVOKABLE int Config::GetGameId()
	{
		static int i = 0;
		if (i < _MaxGameBarLen && i < _SortedId.size())
		{
			int id = _SortedId.at(i++);
			return id;
		}
		i = 0;
		return 0; // end
	}

	Q_INVOKABLE QString Config::GetGameTitle(int id)
	{
		return GetGame(id).Title;
	}

	Q_INVOKABLE QString Config::GetConfigJsonUrl()
	{
		return "";
	}

	Q_INVOKABLE QString Config::GetCurrentDirectory() { return "file:///" + QDir::currentPath() + "/.."; }

	Q_INVOKABLE QString Config::GetGamePresentationUrl(int id)
	{
		auto &game = GetGame(id);
		return game.PresentationUrl;
	}

	Q_INVOKABLE QString Config::GetPresentationFile(int id)
	{
		auto &game = GetGame(id);
		auto &url = game.PresentationUrl;
		if (!url.isEmpty())
		{ // its http link or locak html file
			return "WebGamePage.qml";
		}
		else
		{ // local qml file
			auto full = GameInfoDir(game.Id) + game.PresentationQml;
			return full;
		}
	}

	Q_INVOKABLE int Config::GetDefaultGameId()
	{
		if (_SortedId.size())
			return _SortedId.at(0);
		throw std::exception("None games provided");
	}
}

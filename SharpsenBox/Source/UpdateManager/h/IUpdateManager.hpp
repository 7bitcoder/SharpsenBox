#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <filesystem>
#include "IComponent.hpp"

namespace sb {
	class Game;
}
namespace sb {
	class UpdateInfo;

	struct IUpdateManager {
		enum class State : int {
			NONE = -1, CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET, UPDATING_GAME_PAGES
		};
		enum class VisibleState : int {
			HIDDEN = 0, SHOWED, MINIMALIZED
		};

		virtual ~IUpdateManager() {};

		virtual bool installMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) = 0;
		virtual bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) = 0;
		virtual bool installGame(Game& game, const QString& gamePath, bool shortcut) = 0;
		virtual bool updateGame(Game& game) = 0;

		virtual void downloadStatus(qint64 progress, qint64 total, double speed) = 0;

		virtual void installStatus(qint64 progress) = 0;

		virtual UpdateInfo& getUpdateInfo() = 0;

		virtual void paused() = 0;
		virtual void resumed() = 0;
		virtual void stop() = 0;

	};
}

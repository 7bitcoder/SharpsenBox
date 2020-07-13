#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <filesystem>
#include "IComponent.hpp"

namespace lb {
	enum State : int;
	enum VisibleState : int;
}

namespace upd {
	enum State : int;
}

namespace cf {
	class Game;
}
namespace im {
	class UpdateInfo;

	struct IInstalationManager : public  bc::IComponent, public QThread {
	public:
		virtual ~IInstalationManager() {};

		virtual bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl, bool fullInstall) = 0;
		virtual bool updateGame(cf::Game& game) = 0;

		virtual void downloadStatus(qint64 progress, qint64 total, double speed) = 0;

		virtual void installStatus(qint64 progress) = 0;

		virtual UpdateInfo& getUpdateInfo() = 0;
	};
}

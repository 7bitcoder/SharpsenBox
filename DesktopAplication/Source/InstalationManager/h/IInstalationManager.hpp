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

		virtual  void errorEmit(const QString& errorStr) = 0;
		virtual  void updateProgress(double prog) = 0;

		// AppUpdater
		virtual void updateStatus(upd::State needUpdate) = 0;
		virtual void readGameInfo() = 0;
		virtual void updateEnded(const QString& finalVersion) = 0;

		// loadingBar
		virtual void setTotalLb(double tot) = 0;
		virtual void setActualLb(double act) = 0;
		virtual void setSpeedLb(double sp) = 0;
		virtual void setStateLb(lb::State st) = 0;
		virtual void setVisibleStateLb(lb::VisibleState st) = 0;
		virtual void setUninstallModeLb(bool un) = 0;
	};
}

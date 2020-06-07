#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "InstalationManager.hpp"
#include <filesystem>
#include "Config.hpp"


namespace upd {
	class AppUpdater : public QObject {
		Q_OBJECT
	public:
		enum State : int {
			none = 0, downloading, installing, ended, error, noUpdateFound
		};
		Q_PROPERTY(QString statusStr READ getStateStr);
		Q_PROPERTY(int progress READ getProgress);
		Q_PROPERTY(int updateState READ getState NOTIFY stateChanged);

		Q_INVOKABLE QString getStateStr() const { return statusStr_; }
		Q_INVOKABLE int getProgress() const { return im.getProgress(); }
		Q_INVOKABLE int getState() const { return static_cast<int>(state_); }

		AppUpdater();
		virtual ~AppUpdater() {};
	private:
		void installUpdate();
	public slots:
		void LauchBoxJsonDownloaded();
		void updateDownloaded();
		void updateInstalled();
		void checkForUpdates();
		void errorCatched();
	signals:
		void stateChanged();

	private:
		cf::Config& cf;
		bb::InstalationManager& im;
		QString statusStr_;
		State state_ = State::none;
		bb::InstalationManager* dm = nullptr;
		std::filesystem::path LBJsonFileName = "LaunchBoxInfo.json";
		QString UpdateFile;
		qint64 updateSize_ = 0;
	};
}
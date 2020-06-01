#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "InstalationManager.hpp"
#include <filesystem>


namespace upd {
	class AppUpdater : public QObject {
		Q_OBJECT
	public:
		enum State : int {
			none = 0, downloading, installing, ended, error, noUpdateFound
		};
		Q_PROPERTY(QString statusStr READ getStateStr NOTIFY stateStrChanged);
		Q_PROPERTY(qint64 totalSize READ getTotalSize);
		Q_PROPERTY(qint64 progress READ getProgress);

		Q_INVOKABLE QString getStateStr() const { return statusStr_; }
		Q_INVOKABLE qint64 getTotalSize() const { return updateSize_; }
		Q_INVOKABLE qint64 getProgress() const { return progress_; }

		AppUpdater();
		virtual ~AppUpdater() {};

	public slots:
		void LauchBoxJsonDownloaded();
		void updateDownloaded();
		void checkForUpdates();
		void errorCatched();
	signals:
		void stateStrChanged();

	private:
		void downloadUpdate();
		QString statusStr_ = "Searching for updates";
		State state_ = State::none;
		bool installing_ = false;
		bb::InstalationManager* dm = nullptr;
		std::filesystem::path LBJsonFileName = "LaunchBoxInfo.json";
		qint64 progress_ = 0;
		qint64 updateSize_ = 0;
	};
}
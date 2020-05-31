#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "DownloadManager.hpp"
#include <filesystem>


namespace upd {
	class AppUpdateChecker : public bc::IQmlObject {
		Q_OBJECT
	public:
		static AppUpdateChecker& getObject() {
			static AppUpdateChecker uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override;
		void init() override;
		std::string getName() override;

	private:
		AppUpdateChecker() {}
		virtual ~AppUpdateChecker() {};

	public slots:
		void checkForUpdates();
		void LauchBoxJsonDownloaded();
	signals:
		//void progresChanged();
	private:
		qint64 progress_ = 0;
		qint64 total_ = 0;
		bb::DownloadManager* dm = nullptr;
		std::string LBJsonUrl = "ftp://localhost/LaunchBoxInfo.json";
		std::string LBJsonFileName = "LaunchBoxInfo.json";
	};
}
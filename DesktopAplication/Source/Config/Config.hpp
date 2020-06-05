#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include <filesystem>


namespace cf {
	class Config : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Config& getObject() {
			static Config uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override {};
		void init() override;
		std::string getName() override;

		QString& getVer() { return version_; }
		std::filesystem::path& getDownloadDir() { return downloadDir_; }
		std::filesystem::path& getConfigJson() { return configJson_; }
		std::string& getFtpDir() { return ftpUrl_; }
	private:
		Config();
		virtual ~Config() {};
	private:
		QString version_;
		std::filesystem::path configJson_ = "./LaunchBoxInfo.json";
		std::filesystem::path downloadDir_ = "../Download";
		std::string ftpUrl_ = "ftp://localhost/";
	};
}
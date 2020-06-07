#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include <filesystem>
#include <vector>


namespace cf {
	class Game {
	public:
		int id;
		QString name;
		QString version;
		size_t size;
		QString url;
		bool installed;
		QString gameDir;
		QString execDir;
		std::unordered_map<QString, int> sha;
	};

	class Config : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Config& getObject() {
			static Config uc;
			return uc;
		}
		Q_INVOKABLE bool installed(int id) const { 
			auto it = games_.find(id);
			if (it != games_.end())
				return it->second.installed;
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
		virtual ~Config();
		Game readGameInfo(std::filesystem::path path);
	private:
		QString version_;
		std::filesystem::path configJson_ = "./LaunchBoxInfo.json";
		std::filesystem::path downloadDir_ = "../Download";
		std::string ftpUrl_ = "ftp://localhost/";
		std::unordered_map<int, Game> games_;
	};
}
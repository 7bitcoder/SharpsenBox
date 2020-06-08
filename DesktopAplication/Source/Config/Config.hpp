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
		bool installed;
		bool shortcut;
		size_t size;
		QString name;
		QString version;
		QString url;
		QString gameDir;
		QString execDir;
		QString shortcutPath;
		std::unordered_map<QString, int> sha;
		bool operator==(const Game& g) {
			return id == g.id;
		}
	};

	class Config : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Config& getObject() {
			static Config uc;
			return uc;
		}
		Q_INVOKABLE bool installed(int id) const;
		Q_INVOKABLE QString gamePath(int id) const;
		Q_INVOKABLE void setGamevariables(int id, QString path, bool shortcut);

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
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

		QString& getVer() { return version; }
	private:
		Config() {}
		virtual ~Config() {};
	private:
		QString version;
		std::filesystem::path ConfigJson = "./LaunchBoxInfo.json";
	};
}
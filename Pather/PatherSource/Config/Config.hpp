#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"
#include "TreeModel.hpp"
#include <filesystem>
#include <vector>


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
		void init() override {};
		std::string getName() override { return TYPENAME(Config); };


		//QMl invoklabes

		Q_INVOKABLE bool exists(QString str) { return std::filesystem::exists(str.toStdString()); }

	private:
		Config();
		virtual ~Config() {};
	private:

	};
}

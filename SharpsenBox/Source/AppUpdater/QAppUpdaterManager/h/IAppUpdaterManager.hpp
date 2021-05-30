#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IConfig.hpp"
#include <filesystem>


namespace sb {
	struct IAppUpdaterManager : public IComponent {
	public:
		virtual ~IAppUpdaterManager() {};
	private:
	};
}

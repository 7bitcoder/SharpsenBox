#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include "IConfig.hpp"
#include <filesystem>


namespace upd {
	struct IAppUpdaterManager : public bc::IComponent {
	public:
		virtual ~IAppUpdaterManager() {};
	private:
	};
}

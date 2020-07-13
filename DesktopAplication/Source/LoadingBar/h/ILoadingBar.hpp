#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include <filesystem>
#include "LoadingBarStates.hpp"

namespace lb {
	struct ILoadingBar : public bc::IQmlComponent {
	public:
		virtual ~ILoadingBar() {};
		ILoadingBar() {};

		virtual void reset() = 0;
	};
}

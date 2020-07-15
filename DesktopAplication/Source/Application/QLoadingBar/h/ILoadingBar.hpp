#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IComponent.hpp"
#include <filesystem>

namespace lb {
	struct ILoadingBar : public bc::IComponent {
	public:
		virtual ~ILoadingBar() {};
		ILoadingBar() {};

		virtual void reset() = 0;
	};
}

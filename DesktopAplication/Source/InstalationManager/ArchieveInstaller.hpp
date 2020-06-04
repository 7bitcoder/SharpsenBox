#pragma once 
#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "IQmlObject.hpp"
#include <filesystem>


namespace bb {
	class ArchieveInstaller : public  QThread {
		Q_OBJECT
	public:
		using files = std::vector < std::filesystem::path>;
		ArchieveInstaller() {};
		~ArchieveInstaller() {}

		void run() override;
	signals:

	public slots:
	};
}
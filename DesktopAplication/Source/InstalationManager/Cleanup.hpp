#pragma once 
#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "IQmlObject.hpp"
#include <filesystem>
#include <fstream>

namespace cu {
	class Cleanup : public  QThread {
		Q_OBJECT
	public:

		Cleanup() {};
		virtual ~Cleanup() {}

		// interface
		void run() override;
	signals:
		void ended();
		void error(int);
	public slots:

	private:
	};
}

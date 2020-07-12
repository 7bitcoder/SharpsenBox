#pragma once 
#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "ImElement.hpp"
#include <filesystem>
#include <fstream>

namespace im {
	class Cleanup : public ImElement {
		Q_OBJECT
	public:

		Cleanup() {};
		virtual ~Cleanup() {}

	signals:
		void ended();
		void error(int);
	public slots:

	private:
	};
}

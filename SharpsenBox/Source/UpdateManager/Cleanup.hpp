#pragma once 
#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "IRunnable.hpp"
#include <filesystem>
#include <fstream>

namespace sb {
	class Cleanup : public IRunnable {
	public:

		Cleanup() {};
		virtual ~Cleanup() {}

		// ImElement implementation
		void Run() override;
		void Reset() override {};
	private:
	};
}

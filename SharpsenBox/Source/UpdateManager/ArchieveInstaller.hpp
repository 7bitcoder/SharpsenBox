#pragma once 
#include <iostream>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <atomic>
#include "IComponent.hpp"
#include <filesystem>
#include <fstream>
#include "IRunnable.hpp"

#define BLOCK_SIZE 512000 //~500KB

struct archive;

namespace cf {
	struct AppPack;
}

namespace im {
	class ArchieveInstaller : public IRunnable {
	public:
		ArchieveInstaller() {};
		virtual ~ArchieveInstaller() {}

		// interface
		void resetInstalationDir() { destinationDir_.clear(); }

		// ImElement implementation
		void run() override;
		void reset() override;

	private:
		bool checkState();
		std::string getErrorStr(int code);
		void emitStatus();

		static SSIZE_T readFile(archive* a, void* client_data, const void** buff);
		static int closeFile(archive* a, void* client_data);

		std::filesystem::path destinationDir_;
		std::ifstream file;
		std::string actualUnpacking;
		char buff[BLOCK_SIZE];
		qint64 alreadyRead_ = 0;
		size_t size = 0;
		int res = 0;

		bool cancelled_ = false;
	};
}

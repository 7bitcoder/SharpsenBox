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
#define BLOCK_SIZE 512000 //~500KB
struct archive;
namespace bb {
	class ArchieveInstaller : public  QThread {
		Q_OBJECT
	public:
		struct data {
			std::filesystem::path InstallationDir;
			std::string fileName;
		};
		using files = std::vector < std::pair<std::filesystem::path, std::string>>;

		ArchieveInstaller() {};
		virtual ~ArchieveInstaller() {}

		// interface
		void setUnpackFiles(files files);
		void setInstalationDir(std::filesystem::path dir) { destinationDir_ = dir; }
		void resetInstalationDir() { destinationDir_ .clear(); }
		void run() override;
		void reset();
	signals:
		void statusSignal(qint64 progress);
		void ended();
		void error(int);
	public slots:

	private:
		void emitStatus();
		static int64_t myread(archive* a, void* client_data, const void** buff);
		static int ArchieveInstaller::myclose(archive* a, void* client_data);
		std::filesystem::path destinationDir_;
		files filesToUnpack_;
		std::ifstream file;
		std::string actualUnpacking;
		char buff[BLOCK_SIZE];
		qint64 alreadyRead_ = 0;
		size_t size;
		int res = 0;
	};
}

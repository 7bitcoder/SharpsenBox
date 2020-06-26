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
#include "TreeModel.hpp"

#define BLOCK_SIZE 8192 //~500KB
struct archive;
namespace bb {
	class Packer {
	public:
		using files = std::vector < std::pair<std::filesystem::path, std::string>>;

		Packer() {};
		virtual ~Packer() {}

		// interface
		void setUnpackFiles(files files);
		void setup(std::string& packetName);
		void write(std::string& file, std::string& path, bool dir);
		void setInstalationDir(std::filesystem::path dir) { destinationDir_ = dir; }
		void end();
		void reset();
	private:
		void emitStatus();
		std::filesystem::path destinationDir_;
		files filesToPack_;
		char buff[BLOCK_SIZE];
		qint64 alreadyRead_ = 0;
		dt::TreeModel* root_;
		size_t size;
		int res = 0;
		struct archive* a;
		struct archive_entry* entry;
		struct stat st;
		std::string cmd_;
		int len;
		int fd;
	};
}

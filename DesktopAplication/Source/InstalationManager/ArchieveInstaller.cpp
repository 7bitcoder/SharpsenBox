#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include "ArchieveInstaller.hpp"
#include "Config.hpp"

namespace im {
	SSIZE_T ArchieveInstaller::readFile(::archive* a, void* client_data, const void** buff) {
		ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		*buff = data->buff;
		data->file.read(data->buff, BLOCK_SIZE);
		auto len = data->file.gcount();
		data->alreadyRead_ += len;
		//std::cout << data->alreadyRead << "/" << data->size << "\n";
		data->emitStatus();
		return len;
	}

	int ArchieveInstaller::closeFile(::archive* a, void* client_data) {
		struct ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		if (data->file.is_open())
			data->file.close();
		return (ARCHIVE_OK);
	}

	bool ArchieveInstaller::run() {
		::archive* a;
		::archive_entry* entry;
		int flags;
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;
		try {
			auto& downloadDir = cf::Config::getObject().getDownloadDir();
			auto& filesToUnpack = updateInfo_->getFiles();
			for (auto& arch : filesToUnpack) {
				a = archive_read_new();
				auto actualUnpacking = (downloadDir / arch.fileName).generic_string();

				file.open(actualUnpacking, std::ios::binary);
				if (!file.is_open())
					throw std::exception("Could not open file");
				if (!destinationDir_.empty() && !std::filesystem::exists(destinationDir_.generic_string()))
					std::filesystem::create_directories(destinationDir_.generic_string());

				res = archive_read_support_compression_all(a);
				res = archive_read_support_format_all(a);
				res = archive_read_open(a, this, NULL, ArchieveInstaller::readFile, ArchieveInstaller::closeFile);

				while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
					const char* currentFile = archive_entry_pathname(entry);
					std::filesystem::path fullOutputPath;
					// if global destination is not set = games info so grab destination from config
					if (destinationDir_.empty() && !arch.destination.empty()) {
						fullOutputPath = arch.destination;
						if (!fullOutputPath.empty() && !std::filesystem::exists(fullOutputPath.generic_string()))
							std::filesystem::create_directories(fullOutputPath.generic_string());
					} else
						fullOutputPath = destinationDir_.generic_string();

					if (currentFile) {
						fullOutputPath /= currentFile;
						fullOutputPath = fullOutputPath.generic_string();
						archive_entry_set_pathname(entry, fullOutputPath.generic_string().c_str());
						res = archive_read_extract(a, entry, flags);
						if (res != ARCHIVE_OK) {
							//archive_read_finish(a);
							break;
						}
					} else {
						//error with empty path
					}
				}
				res = archive_read_finish(a);
			}
		} catch (std::filesystem::filesystem_error& e) {
			res = -1;
		} catch (...) {
			res = ARCHIVE_FAILED;
		}

		if (res != ARCHIVE_OK) {
			setErrorStr(res);
			return false;
		}
		return true;
	}

	void ArchieveInstaller::emitStatus() {
		im_->installStatus(alreadyRead_);
	}

	void ArchieveInstaller::reset() {
		destinationDir_ = "";
		filesToUnpack_.clear();
		actualUnpacking = "";

		alreadyRead_ = 0;
		size = 0;
		res = 0;
	}

	void ArchieveInstaller::setErrorStr(int code) {
		switch (code) {
		case ARCHIVE_FATAL:
			errorStr_ = "Installing Fatal error, check memory disk space";
			break;
		case -1:
			errorStr_ = "Filesystem error, check memory disk space";
			break;
		case ARCHIVE_EOF:
		case ARCHIVE_WARN:
		case ARCHIVE_FAILED:
		default:
			errorStr_ = "Unexpected Error ocured while installing files";
			break;
		}
	}
}



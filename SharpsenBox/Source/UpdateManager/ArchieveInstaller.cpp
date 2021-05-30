#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <QDir>
#include <archive.h>
#include <archive_entry.h>
#include "ArchieveInstaller.hpp"
#include "IConfig.hpp"
#include "UpdateInfo.hpp"

namespace sb {
	SSIZE_T ArchieveInstaller::readFile(::archive* a, void* client_data, const void** buff) {
		ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		*buff = data->buff;
		data->file.read(data->buff, BLOCK_SIZE);
		auto len = data->file.gcount();
		data->alreadyRead_ += len;
		//std::cout << data->alreadyRead << "/" << data->size << "\n";
		data->emitStatus();
		if (!data->checkState()) {
			return -1;
		}
		return len;
	}

	bool ArchieveInstaller::checkState() {
		if (!UpdateInfo->pause.test_and_set()) {
			UpdateManager->paused();
			while(1) {// wait for
				QThread::currentThread()->usleep(1000);
				if (!UpdateInfo->resume.test_and_set()) {
					UpdateManager->resumed();
					break;
				} else if (!UpdateInfo->stop.test_and_set() || cancelled_) {
					return false;
				}
			}
		} else if (!UpdateInfo->stop.test_and_set() || cancelled_) {
			std::cout << "cancel\n";
			cancelled_ = true;
			return false;
		}
		return true;
	}

	int ArchieveInstaller::closeFile(::archive* a, void* client_data) {
		class ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		if (data->file.is_open())
			data->file.close();
		return (ARCHIVE_OK);
	}

	void ArchieveInstaller::Run() {
		::archive* a;
		::archive_entry* entry;
		int flags;
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;
		try {
			destinationDir_ = UpdateInfo->getInstallDir().toUtf8().constData();
			auto& config = Component<IConfig>::Get();
			auto& downloadDir = config.GetDownloadDir();
			auto& filesToUnpack = UpdateInfo->getFiles();
			for (auto& arch : filesToUnpack) {
				a = archive_read_new();
				auto actualUnpacking = config.CombinePath({downloadDir, arch.FileName});

				file.open(actualUnpacking.toStdString(), std::ios::binary);
				if (!file.is_open())
					throw std::exception("Could not open file");
				if (!destinationDir_.empty() && !std::filesystem::exists(destinationDir_.generic_string()))
					std::filesystem::create_directories(destinationDir_);

				res = archive_read_support_compression_all(a);
				res = archive_read_support_format_all(a);
				res = archive_read_open(a, this, NULL, ArchieveInstaller::readFile, ArchieveInstaller::closeFile);

				while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
					const char* currentFile = archive_entry_pathname(entry);
					std::filesystem::path fullOutputPath;
					// if global destination is not set = games info so grab destination from config
					if (destinationDir_.empty() && !arch.Destination.isEmpty()) {
						fullOutputPath = arch.Destination.toStdString();
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
		} catch (std::filesystem::filesystem_error&) {
			res = -1;
		} catch (...) {
			res = ARCHIVE_FAILED;
		}
		if (cancelled_)
			throw AbortException();
		if (res != ARCHIVE_OK) {
			Error(getErrorStr(res));
		}
	}

	void ArchieveInstaller::emitStatus() {
		UpdateManager->installStatus(alreadyRead_);
	}

	void ArchieveInstaller::Reset() {
		destinationDir_ = "";
		actualUnpacking = "";

		alreadyRead_ = 0;
		size = 0;
		res = 0;
		cancelled_ = false;
	}

	std::string ArchieveInstaller::getErrorStr(int code) {
		switch (code) {
		case ARCHIVE_FATAL:
			return "Installing Fatal error, check memory disk space";
		case -1:
			return "Filesystem error, check memory disk space";
		case ARCHIVE_EOF:
		case ARCHIVE_WARN:
		case ARCHIVE_FAILED:
		default:
			return "Unexpected Error ocured while installing files";
		}
		return "";
	}
}



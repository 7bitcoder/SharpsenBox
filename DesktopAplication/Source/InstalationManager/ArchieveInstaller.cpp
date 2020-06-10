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

namespace bb {
	void ArchieveInstaller::setUnpackFiles(std::vector<std::filesystem::path> files) {
		filesToUnpack = files;
	}

	SSIZE_T ArchieveInstaller::myread(::archive* a, void* client_data, const void** buff) {
			ArchieveInstaller* data = (ArchieveInstaller*)client_data;
			*buff = data->buff;
			data->file.read(data->buff, BLOCK_SIZE);
			auto len = data->file.gcount();
			data->alreadyRead += len;
			//std::cout << data->alreadyRead << "/" << data->size << "\n";
			data->emitStatus();
			return len;
	}

	int ArchieveInstaller::myclose(::archive* a, void* client_data) {
		struct ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		if (data->file.is_open())
			data->file.close();
		return (ARCHIVE_OK);
	}

	void ArchieveInstaller::run() {
		::archive* a;
		::archive_entry* entry;
		int flags;
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;
		try {
			auto& downloadDir = cf::Config::getObject().getDownloadDir();
			for (auto& arch : filesToUnpack) {
				a = archive_read_new();
				auto actualUnpacking = (downloadDir / arch).generic_string();
				size = std::filesystem::file_size(actualUnpacking);
				file.open(actualUnpacking, std::ios::binary);
				if (!file.is_open())
					throw std::exception("Could not open file");
				res = archive_read_support_compression_all(a);
				res = archive_read_support_format_all(a);
				res = archive_read_open(a, this, NULL, ArchieveInstaller::myread, ArchieveInstaller::myclose);
				while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
					//printf("%s\n", archive_entry_pathname(entry));
					const char* currentFile = archive_entry_pathname(entry);
					std::filesystem::path fullOutputPath = destinationDir_.generic_string();
					fullOutputPath /= currentFile;
					fullOutputPath = fullOutputPath.generic_string();
					archive_entry_set_pathname(entry, fullOutputPath.generic_string().c_str());
					res = archive_read_extract(a, entry, flags);
					if (res != ARCHIVE_OK) {
						archive_read_finish(a);
						break;
					}
				}
				res = archive_read_finish(a);
			}

			try {
				std::filesystem::remove_all(downloadDir / ".");
			} catch (...) {
				res = -1;//errorCatched(-1);
			}
		} catch ( ... ) {}

		if (res != ARCHIVE_OK) {
			emit error(res);
		} else {
			ended();
		}
	}

	void ArchieveInstaller::emitStatus() {
		statusSignal(alreadyRead);
	}
}
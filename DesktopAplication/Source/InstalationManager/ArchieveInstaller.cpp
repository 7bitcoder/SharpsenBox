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

namespace bb {
	void ArchieveInstaller::setUnpackFiles(std::vector<std::filesystem::path> files) {
		filesToUnpack = files;
	}

	SSIZE_T ArchieveInstaller::myread(struct archive* a, void* client_data, const void** buff) {
			ArchieveInstaller* data = (ArchieveInstaller*)client_data;
			*buff = data->buff;
			data->file.read(data->buff, SIZE);
			auto len = data->file.gcount();
			data->alreadyRead += len;
			std::cout << data->alreadyRead << "/" << data->size << "\n";
			return len;
	}

	int ArchieveInstaller::myclose(struct archive* a, void* client_data) {
		struct ArchieveInstaller* data = (ArchieveInstaller*)client_data;
		if (data->file.is_open())
			data->file.close();
		return (ARCHIVE_OK);
	}

	void ArchieveInstaller::run() {
		archive* a;
		archive_entry* entry;
		int flags;
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;
		for (auto& arch : filesToUnpack) {
			a = archive_read_new();
			size = std::filesystem::file_size(actualUnpacking);
			file.open(actualUnpacking, std::ios::binary);
			if (!file.is_open())
				throw std::exception("Could not open file");
			archive_read_support_compression_all(a);
			archive_read_support_format_all(a);
			archive_read_open(a, this, NULL, ArchieveInstaller::myread, ArchieveInstaller::myclose);
			while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
				//printf("%s\n", archive_entry_pathname(entry));
				archive_read_extract(a, entry, flags);
			}
			archive_read_finish(a);
		}
	}
	void ArchieveInstaller::emitStatus() {
		statusSignal(alreadyRead);
	}
}
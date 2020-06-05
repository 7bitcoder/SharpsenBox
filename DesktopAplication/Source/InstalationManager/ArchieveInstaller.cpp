#include "ArchieveInstaller.hpp"
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

#define SIZE 512000
namespace bb {
	namespace {
		struct mydata {
			const char* name = "bin.rar";
			std::ifstream file;
			char buff[SIZE];
			size_t alreadyRead = 0;
			size_t size;
		};

		SSIZE_T myread(struct archive* a, void* client_data, const void** buff) {
			mydata* data = (mydata*)client_data;
			*buff = data->buff;
			data->file.read(data->buff, SIZE);
			auto len = data->file.gcount();
			data->alreadyRead += len;
			std::cout << data->alreadyRead << "/" << data->size << "\n";
			return len;
		}

		int myclose(struct archive* a, void* client_data) {
			struct mydata* data = (mydata*)client_data;
			if (data->file.is_open())
				data->file.close();
			std::cout << "DONE!!!\n";
			return (ARCHIVE_OK);
		}
	}

	void ArchieveInstaller::run() {
		struct mydata* data;
		struct archive* a;
		struct archive_entry* entry;
		data = new mydata;
		int flags;
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;
		a = archive_read_new();
		data->size = std::filesystem::file_size(data->name);
		data->file.open(data->name, std::ios::binary);
		if (!data->file.is_open())
			throw std::exception("Could not open file");
		archive_read_support_compression_all(a);
		archive_read_support_format_all(a);
		archive_read_open(a, data, NULL, myread, myclose);
		while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
			//printf("%s\n", archive_entry_pathname(entry));
			archive_read_extract(a, entry, flags);
		}
		archive_read_finish(a);
		delete data;
	}
}
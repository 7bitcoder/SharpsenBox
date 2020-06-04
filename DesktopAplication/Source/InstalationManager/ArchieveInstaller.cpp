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

static int siz = 0;
namespace bb {
	namespace {
		int copy_data(struct archive* ar, struct archive* aw) {
			int r;
			const void* buff;
			size_t size;
			la_int64_t offset;

			for (;;) {
				r = archive_read_data_block(ar, &buff, &size, &offset);
				if (r == ARCHIVE_EOF)
					return (ARCHIVE_OK);
				if (r < ARCHIVE_OK)
					return (r);
				r = archive_write_data_block(aw, buff, size, offset);
				if (r < ARCHIVE_OK) {
					fprintf(stderr, "%s\n", archive_error_string(aw));
					return (r);
				}
			}

		}

		struct mydata {
			const char* name = "amd_chipset_drivers_am4_tr4.zip";
			std::ifstream file;
			char buff[1024];
			size_t alreadyRead = 0;
			size_t size;
		};

		SSIZE_T myread(struct archive* a, void* client_data, const void** buff) {
			mydata* data = (mydata*)client_data;
			*buff = data->buff;
			data->file.read(data->buff, 1024);
			auto len = data->file.gcount();
			data->alreadyRead += len;
			std::cout << data->alreadyRead << "/" << data->size << "\n";
			return len;
		}

		int myclose(struct archive* a, void* client_data) {
			struct mydata* data = (mydata*)client_data;
			if (data->file.is_open())
				data->file.close();
			return (ARCHIVE_OK);
		}

		struct progress_data {
			struct archive* archive;
			struct archive_entry* entry;
		};

		void progress_func(void* cookie) {
			progress_data* data = static_cast<progress_data*>(cookie);
			archive* a = data->archive;
			archive_entry* entry = data->entry;
			uint64_t comp, uncomp;
			int compression;

			if (a) {

				auto i = archive_position_compressed(a);
				auto ji = archive_position_uncompressed(a);
				comp = archive_filter_bytes(a, -1);
				uncomp = archive_filter_bytes(a, 0);
				siz += i;
				if (comp > uncomp)
					compression = 0;
				else
					compression = (int)((uncomp - comp) * 100 / uncomp);
				fprintf(stderr,
					"In: %jd bytes, compression %d%%\n;",
					(intmax_t)comp, compression);
			}
			if (entry) {
				fprintf(stderr, "Current: %s\n",
					archive_entry_pathname(entry));
				fprintf(stderr, " (%d bytes)\n",
					(archive_entry_size(entry)));
			}
		}


	}

	void ArchieveInstaller::run() {
		struct mydata* data;
		struct archive* a;
		struct archive_entry* entry;
		progress_data pd;
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
		//archive_read_support_filter_all(a);
		pd.archive = a;
		pd.entry = nullptr;
		archive_read_extract_set_progress_callback(a, progress_func, &pd);
		while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
			printf("%s\n", archive_entry_pathname(entry));
			pd.entry = entry;
			archive_read_extract(a, entry, flags);
		}
		archive_read_finish(a);
		delete data;
	}
}
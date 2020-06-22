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
#include "Packer.hpp"
#include "Config.hpp"

namespace bb {

	void Packer::setup(std::string& packetName) {
		a = archive_write_new();
		auto res = archive_write_add_filter_gzip(a);
		//std::cout << archive_error_string(a);
		res = archive_write_set_format_pax_restricted(a); // Note 1
		res = archive_write_open_filename(a, packetName.c_str());
	}

	void Packer::write(std::string& file, std::string& path) {
		stat(file.c_str(), &st);
		entry = archive_entry_new(); // Note 2
		archive_entry_set_pathname(entry, path.c_str());
		archive_entry_set_size(entry, st.st_size); // Note 3
		archive_entry_set_filetype(entry, AE_IFREG);
		archive_entry_set_perm(entry, 0644);
		auto res = archive_write_header(a, entry);
		fd = open(file.c_str(), O_RDONLY);
		len = read(fd, buff, sizeof(buff));
		while (len > 0) {
			archive_write_data(a, buff, len);
			len = read(fd, buff, sizeof(buff));
		}
		close(fd);
		archive_entry_free(entry);
	}

	void Packer::end() {
		auto res = archive_write_close(a); // Note 4
		auto ress = archive_write_free(a); // Note 5
	}
}

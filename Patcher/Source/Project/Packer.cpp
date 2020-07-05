#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <zip.h>
#include "Packer.hpp"
#include "Config.hpp"

namespace bb {
	zip_t* zipper_;
	void Packer::setup(std::string& packetName) {
		int errorp;
		if (std::filesystem::exists(packetName))
			std::filesystem::remove(packetName);
		zipper_ = zip_open(packetName.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
		if (zipper_ == nullptr) {
			zip_error_t ziperror;
			zip_error_init_with_code(&ziperror, errorp);
			throw std::runtime_error("Failed to open output file " + packetName + ": " + zip_error_strerror(&ziperror));
		}
	}

	void Packer::write(std::string& file, std::string& path, bool dir) {
		if (dir) {
			if (zip_dir_add(zipper_, path.c_str(), ZIP_FL_ENC_UTF_8) < 0) {
				throw std::runtime_error("Failed to add directory to zip: " + std::string(zip_strerror(zipper_)));
			}
		} else {
			zip_source_t* source = zip_source_file(zipper_, file.c_str(), 0, 0);
			if (source == nullptr) {
				throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper_)));
			}
			if (zip_file_add(zipper_, path.c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
				zip_source_free(source);
				throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper_)));
			}
		}
	}

	void Packer::end() {
		zip_close(zipper_);
	}
}

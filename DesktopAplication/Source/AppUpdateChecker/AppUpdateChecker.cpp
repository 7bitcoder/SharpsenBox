#include "AppUpdateChecker.hpp"
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

namespace upc {

	namespace {
		struct FtpFile {
			const char* filename;
			FILE* stream;
		};

		static size_t my_fwrite(void* buffer, size_t size, size_t nmemb, void* stream) {
			struct FtpFile* out = (struct FtpFile*)stream;
			if (!out->stream) {
				/* open file for writing */
				out->stream = fopen(out->filename, "wb");
				if (!out->stream)
					return -1; /* failure, can't open file to write */
			}
			return fwrite(buffer, size, nmemb, out->stream);
		}
	}

	int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
		// It's here you will write the code for the progress message or bar
		std::cout << "TotalToDownload: " << TotalToDownload << "\tNowDownloaded: " << NowDownloaded << "\tTotalToUpload: " << TotalToUpload << "\tNowUploaded: " << NowUploaded << "\n";
		return 0;
	}

	void AppUpdateChecker::checkForUpdates() {
		CURL* curl;
		CURLcode res;
		struct FtpFile ftpfile = {
		  "curl.tar.gz", /* name to store the file as if successful */
		  NULL
		};

		curl_global_init(CURL_GLOBAL_DEFAULT);

		curl = curl_easy_init();
		if (curl) {
			/*
			 * You better replace the URL with one that works!
			 */
			curl_easy_setopt(curl, CURLOPT_URL,
				"ftp://speedtest.tele2.net/200MB.zip");
			/* Define our callback to get called when there's data to be written */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
			/* Set a pointer to our struct to pass to the callback */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
			// Install the callback function
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
			/* Switch on full protocol/debug output */
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			res = curl_easy_perform(curl);

			/* always cleanup */
			curl_easy_cleanup(curl);

			if (CURLE_OK != res) {
				/* we failed */
				fprintf(stderr, "curl told us %d\n", res);
			}
		}

		if (ftpfile.stream)
			fclose(ftpfile.stream); /* close the local file */

		curl_global_cleanup();
	}
}
#include "FtpDownloader.hpp"
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
#include <QElapsedTimer>

namespace bb {

	size_t FtpDownloader::my_fwrite(void* buffer, size_t size, size_t nmemb, void* userdata) {
		FtpDownloader* out = (FtpDownloader*)userdata;
		if (!out->stream_) {
			/* open file for writing */
			out->stream_ = fopen(out->outfile_.c_str(), "wb");
			if (!out->stream_)
				return -1; /* failure, can't open file to write */
		}  if (out->cancelled)
			return size == 0 ? 1 : size - 1;

		return  fwrite(buffer, size, nmemb, out->stream_);
	}

	int FtpDownloader::progress_callback(void* userData, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
		// It's here you will write the code for the progress message or bar
		FtpDownloader* out = (FtpDownloader*)userData;
		out->total_ = dltotal;
		out->now_ = dlnow;
		out->emitStatus();
		return out->checkState();
	}

	int FtpDownloader::checkState() {
		if (!pause.test_and_set()) {
			res = curl_easy_pause(curl, CURLPAUSE_ALL);
			if (res == CURLE_OK)
				;//emit ok 
			std::cout << "pause2\n";
		} else if (!resume.test_and_set()) {
			res = curl_easy_pause(curl, CURLPAUSE_CONT);
			if (res == CURLE_OK)
				;//emit ok 
		} else if (!stop.test_and_set() || cancelled) {
			std::cout << "cancel\n";
			cancelled = true;
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
		}
		return 0;
	}

	void FtpDownloader::emitStatus() {
		auto x = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_);
		emit statusSignal(now_, total_, speed_);
	}

	void FtpDownloader::run() {
		try {
			// clear flags etc 
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();
			if (curl) {
				/*
				 * You better replace the URL with one that works!
				 */
				 /* Define our callback to get called when there's data to be written */
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FtpDownloader::my_fwrite);
				/* Set a pointer to our struct to pass to the callback */
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);

				curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
				// Install the callback function
				curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, FtpDownloader::progress_callback);
				/* Switch on full protocol/debug output */

				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
				for (size_t i = 0; i < cancelled && files_.size(); i++, url_ = files_[i]) {
					curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
					res = curl_easy_perform(curl);
					if (CURLE_OK != res) {
						/* we failed */
						if (res == CURLE_OPERATION_TIMEDOUT && cancelled) {
							//ok cancelled 
						} else
							fprintf(stderr, "curl told us %d\n", res);
					}
					if (stream_)
						fclose(stream_); /* close the local file */
					stream_ = nullptr;
					cancelled = false;
				}
				/* always cleanup */
				curl_easy_cleanup(curl);
			}


			curl_global_cleanup();
		} catch (...) {
			fprintf(stderr, "exception catched white doanloading data");
		}
		std::cout << "termination\n";
		emit ended();
	}
}
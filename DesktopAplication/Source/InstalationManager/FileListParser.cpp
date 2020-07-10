#include "FileListParser.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_map>

namespace bb {
	void FileListParser::parse(bool fullInstall) {
		fullInstall_ = fullInstall;
		start();
	}
	
	void FileListParser::run() {
		QString val;
		QFile file;
		//open LaunchBoxConfig file
		auto path = cf::Config::getObject().getDownloadDir() / parseInfoFileName;
		file.setFileName(path.generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		auto& ghj = val.toStdString();
		file.close();
		fileList_ = QJsonDocument::fromJson(val.toUtf8());
		QString ver = fileList_["Ver"].toString();
		auto& ss = ver.toStdString();
		auto& gg = actualVersion_.toStdString();
		if ( false /*ver != toUpdateVersion_ */) { //need update
			//todo error
		} else if (fullInstall_) {
			readAllPackets();
		} else { //app is up to date
			readPackets();
		}
		parseEnded();
	}
	
	void FileListParser::readPackets() {
		auto it = pathFiles_.begin();
		auto& fileList = fileList_["Files"].toObject();
		for (++it; it != pathFiles_.end(); it++) {
			QString val;
			QFile file;
			//open LaunchBoxConfig file
			auto path = cf::Config::getObject().getDownloadDir() / it->fileName;
			file.setFileName(path.generic_string().c_str());
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto& ghj = val.toStdString();
			file.close();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
			auto changed = d["ChangedFiles"].toArray();
			auto removed = d["RemovedFiles"].toArray();
			auto added = d["AddedFiles"].toArray();
			// remove
			for (auto& rem : removed) {
				auto str = rem.toString();
				auto found = toDownload_.find(str);
				if (found != toDownload_.end()) {
					toDownload_.erase(found);
				} else {
					toRemove_.insert(str);
				}
			}

			// add
			for (auto& chan : changed) {
				auto str = chan.toString();
				toDownload_.insert(str);
			}

			for (auto& add : added) {
				auto str = add.toString();
				toDownload_.insert(str);
			}
		}
		auto packets = fileList_["Packets"].toObject();
		
		std::unordered_map<std::string, std::string> neededPackets;
		for (auto& file : toDownload_) {
			auto elem = fileList[file].toObject();
			auto& pack = packets[elem["Id"].toString()].toObject();
			if (!neededPackets.contains(pack["Url"].toString().toStdString())) {
				totalBytesTo_ += std::stoll(pack["Size"].toString().toStdString());
				neededPackets.insert({ pack["Url"].toString().toStdString(), pack["Name"].toString().toStdString() });
			} 
		}

		for (auto pack : neededPackets) {
			files_.push_back({ pack.first, pack.second });
		}
	}

	void FileListParser::readAllPackets() {
		auto packets = fileList_["Packets"].toObject();
		for (auto& pac : packets) {
			auto& pack = pac.toObject();
			auto tt = pack["Url"].toString().toStdString();
			auto sizeStr = pack["Size"].toString().toStdString();
			totalBytesTo_ += std::stoll(sizeStr);
			files_.push_back({ pack["Url"].toString().toStdString(), pack["Name"].toString().toStdString() });
		}
	}

	void FileListParser::reset() {
		actualVersion_ = "";
		files_.clear();
		totalBytesTo_ = 0;
		needUpdate_ = false;
		toDownload_.clear();
		toRemove_.clear();
	}
}
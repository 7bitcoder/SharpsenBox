#pragma once
#include "FileListParser.hpp"
#include "IConfig.hpp"
#include "Game.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_map>
#include "UpdateInfo.hpp"

namespace sb {
	void FileListParser::Run() {
		try {
			QString val;
			QFile file;
			//open SharpsenBoxConfig file
			auto & config = Component<IConfig>::Get();
			auto path = config.CombinePath({config.GetDownloadDir(), parseInfoFileName});
			file.setFileName(path);
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto ghj = val.toStdString();
			file.close();
			fileList_ = QJsonDocument::fromJson(val.toUtf8());
			QString ver = fileList_["Ver"].toString();

			auto ss = ver.toStdString();
			if (false /*ver != toUpdateVersion_ */) { //need Update
				//todo error
			} else if (UpdateInfo->getFullInstall()) {
				readAllPackets();
			} else { //app is up to date
				readPackets();
			}
		} catch (...) {
			Error("Error ocured while processing Update fileList");
		}
	}

	void FileListParser::readPackets() {
		auto& pathFiles = UpdateInfo->getFiles(); // path files 1st is file list so skip it
		auto it = pathFiles.begin();
		auto fileList = fileList_["Files"].toObject();
		for (++it; it != pathFiles.end(); it++) {
			QString val;
			QFile file;
			//open SharpsenBoxConfig file
			auto & config = Component<IConfig>::Get();
			auto path = config.CombinePath({config.GetDownloadDir(), it->FileName});
			file.setFileName(path);
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto ghj = val.toStdString();
			file.close();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
			auto changed = d["ChangedFiles"].toArray();
			auto removed = d["RemovedFiles"].toArray();
			auto added = d["AddedFiles"].toArray();
			// remove
			for (auto rem : removed) {
				auto str = rem.toString();
				auto found = toDownload_.find(str);
				if (found != toDownload_.end()) {
					toDownload_.erase(found);
				} else {
					toRemove_.insert(str);
				}
			}

			// add
			for (auto chan : changed) {
				auto str = chan.toString();
				toDownload_.insert(str);
			}

			for (auto add : added) {
				auto str = add.toString();
				toDownload_.insert(str);
			}
		}
		auto packets = fileList_["Packets"].toObject();

		std::unordered_map<QString, QString> neededPackets;
		for (auto& file : toDownload_) {
			auto elem = fileList[file].toObject();
			auto pack = packets[elem["Id"].toString()].toObject();
			if (!neededPackets.contains(pack["Url"].toString())) {
				totalBytesTo_ += pack["Size"].toString().toLongLong();
				neededPackets.insert({ pack["Url"].toString(), pack["Name"].toString() });
			}
		}

		for (auto pack : neededPackets) {
			files_.push_back({ pack.first, pack.second });
		}
	}

	void FileListParser::readAllPackets() {
		auto packets = fileList_["Packets"].toObject();
		for (auto pac : packets) {
			auto pack = pac.toObject();
			auto sizeStr = pack["Size"].toString();
			totalBytesTo_ += sizeStr.toLongLong();
			files_.push_back({ pack["Url"].toString(), pack["Name"].toString() });
		}
	}

	void FileListParser::Reset() {
		files_.clear();
		totalBytesTo_ = 0;
		toDownload_.clear();
		toRemove_.clear();
	}
}
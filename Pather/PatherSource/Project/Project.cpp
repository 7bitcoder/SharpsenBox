#include "Project.hpp"
#include <filesystem>
#include <iostream>
#include <QCryptographicHash>

namespace pr {
	namespace fs = std::filesystem;
	namespace {
		std::pair<QString, qint64 > fileChecksum(const QString& fileName,
			QCryptographicHash::Algorithm hashAlgorithm) {
			QFile f(fileName);
			qint64 size = f.size();
			if (f.open(QFile::ReadOnly)) {
				QCryptographicHash hash(hashAlgorithm);
				if (hash.addData(&f)) {
					return  { hash.result().toHex(), size };
				}
			}
			return { QString(), size };
		}
	}

	void Project::generate() {
		//rootObject_ = doc_.object();
		//rootObject_.insert("GameName", gameName.c_str());
		//rootObject_.insert("GameDir", gameDir_.generic_string().c_str());
		//rootObject_.insert("ProjectName", projectName.c_str());
		//rootObject_.insert("ProjectDir", projectDir.generic_string().c_str());
		//
		//int index = 0;
		//for (auto& p : fs::recursive_directory_iterator(gameDir_)) {
		//	if (p.is_directory())
		//		continue;
		//	insertFileData(p);
		//	index++;
		//}
		//doc_.setObject(rootObject_);
		//file_.setFileName("fileList.json");
		//file_.open(QIODevice::WriteOnly | QIODevice::Text);
		//file_.write(doc_.toJson());
		//file_.close();
	}
	void Project::insertFileData(std::filesystem::path file) {
		auto data = fileChecksum(file.generic_string().c_str(), QCryptographicHash::Algorithm::Sha3_256);
		if (data.first.isEmpty())
			throw std::exception("errorr while computing sha hash");
		std::string gg = data.first.toStdString();
		std::string dgg = QString::number(data.second).toStdString();
		QJsonObject o;
		o.insert("Sha", data.first);
		o.insert("Size", QString::number(data.second));
		auto relative = std::filesystem::relative(file, rootDir_);
		//rootObject_.insert(relative.generic_string().c_str(), o);
	}

	void Project::save() {
		if (!std::filesystem::exists(projectDir / projectName))
			std::filesystem::create_directories(projectDir / projectName);
		QJsonObject ro = doc_.object();
		ro.insert("AppName", gameName.c_str());
		if (version_.isEmpty()) {
			std::cout << "specify Patch version\n";
			return;
		}
		ro.insert("Ver", version_);

		auto packets = dt::TreeModel::getObject().getPackets();
		QJsonObject appitems;
		for (auto packet : packets) {
			auto& name = packet->getPacketName();
			packer_.setup((projectDir / projectName / name.toStdString()).generic_string());
			std::cout << "packet: " << name.toStdString() << std::endl;
			QJsonObject root;
			insertData(packet->rootItemPtr(), root);
			QJsonObject packet;
			packet.insert("Files", root);
			packet.insert("Url", "Asdasd");
			packet.insert("Size", "Asdasd");
			appitems.insert(name, packet);
			packer_.end();
		}
		ro.insert("AppComponents", appitems);
		doc_.setObject(ro);
		std::filesystem::path filelist = projectDir / projectName / "fileList.json";
		file_.setFileName(filelist.generic_string().c_str());
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();

		ro.insert("AppDir", AppDir_.generic_string().c_str());
		ro.insert("ProjectName", projectName.c_str());
		ro.insert("ProjectDir", projectDir.generic_string().c_str());

		doc_.setObject(ro);
		std::filesystem::path project = projectDir / projectName / (projectName + ".json");
		file_.setFileName(project.generic_string().c_str());
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();
	}

	void Project::insertData(dt::TreeItem* item, QJsonObject& object) {
		auto size = item->childCount();
		for (size_t i = 0; i < size; i++) {
			auto* child = item->child(i);
			std::cout << "file: " << child->path().toStdString() << std::endl;
			QJsonObject file;
			if (child->isDirectory()) {
				insertData(child, file);
			} else {
				file.insert("Size", child->fileSize());
				file.insert("Sha", child->fileSha());
			}
			auto fullPath = AppDir_ / child->path().toStdString();
			object.insert(fullPath.filename().c_str(), file);
			packer_.write(fullPath.generic_string(), child->path().toStdString(), child->isDirectory());
		}
	}

	Q_INVOKABLE void Project::loadProject() {


		gameName = d["AppName"].toString().toStdString();
		version_ = d["Ver"].toString();
		
		projectName = d["ProjectName"].toString().toStdString();
		projectDir = d["ProjectDir"].toString().toStdString();

		auto& packets = dt::TreeModel::getObject().getPackets();
		QJsonObject AppComponents = d["AppComponents"].toObject();
		for (auto& packet : AppComponents) {
			QJsonObject& pack = packet.toObject();
			auto* newPacket = new dt::TreeModel(true); //packet
			std::filesystem::path fullpath;
			readPacket(newPacket->rootItemPtr(), pack["Files"].toObject(), fullpath);
			packets.append(newPacket);
		}
		numberOFPacks = packets.size();
		nmbChanged();
	}

	void Project::readPacket(dt::TreeItem* item, QJsonObject& object, std::filesystem::path& fullPath) {
		for (auto& it = object.begin(); it < object.end(); it++) {
			QJsonObject& file = it->toObject();
			fullPath /= it.key().toStdString();
			QJsonValueRef size = file["Size"];
			if (size.isString() && !size.isObject()) { //file
				auto* appended = item->appendChildren({ it.key(),  fullPath.generic_string().c_str() }, false, file["Sha"].toString(), std::stoll(size.toString().toStdString()));
				verify(appended);
			} else {
				auto* appended = item->appendChildren({ it.key(),  fullPath.generic_string().c_str() }, false, "", 0);
				verify(appended);
				readPacket(appended, file, fullPath);
			}
			fullPath = fullPath.parent_path();
		}
	}

	void Project::verify(dt::TreeItem* item) {
		int size = item->childCount();
		auto& map = dt::TreeModel::getObject().getSetUpModel().getDirFiles();
		auto& path = item->path();
		auto it = map.find(path);
		if (it == map.end()) {//new element
			item->setState(dt::TreeItem::fileState::DELETED);
		} else {
			if (!it->dir && !item->isDirectory()) {
				bool sizeCmp = it->size == item->fileSize(), shaCmp = it->sha == item->fileSha();
				if (!sizeCmp || !shaCmp) {
					item->setState(dt::TreeItem::fileState::CHANGED);
				} else if (sizeCmp && shaCmp) {
					item->setState(dt::TreeItem::fileState::SAME);
				}
			} else {} //error
		}
	}
}

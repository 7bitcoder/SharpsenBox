#include "Project.hpp"
#include <filesystem>
#include <iostream>
#include <QCryptographicHash>
#include <QJsonArray>
#include "Config.hpp"
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
		QJsonObject fileList;
		rootObject_ = &fileList;
		QJsonObject packetsList;
		packetId = 0;
		for (auto packet : packets) {
			auto& name = packet->getPacketName();
			auto packDir = projectDir / projectName / name.toStdString();
			packer_.setup(packDir.generic_string());
			std::cout << "packet: " << name.toStdString() << std::endl;
			QJsonObject root;

			insertData(packet->rootItemPtr(), root);
			QJsonObject packet;
			packet.insert("Files", root);
			packet.insert("Url", "---");
			packet.insert("Size", "---");
			appitems.insert(name, packet);
			packer_.end();

			QJsonObject pack;
			pack.insert("Name", name);
			pack.insert("Url", "---");
			auto size = std::filesystem::file_size(packDir);
			pack.insert("Size", QString::number(static_cast<qint64>(size)));
			packetsList.insert(QString::number(packetId++), pack);
		}
		QJsonObject list;
		list.insert("AppName", gameName.c_str());
		list.insert("Ver", version_);
		list.insert("Files", fileList);
		list.insert("Packets", packetsList);
		doc_.setObject(list);
		std::filesystem::path filelist = projectDir / projectName / "fileList.json";
		file_.setFileName(filelist.generic_string().c_str());
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();

		ro.insert("AppDir", AppDir_.generic_string().c_str());
		ro.insert("ProjectName", projectName.c_str());
		ro.insert("ProjectDir", projectDir.generic_string().c_str());
		ro.insert("AppComponents", appitems);

		doc_.setObject(ro);
		std::filesystem::path project = projectDir / projectName / (projectName + ".json");
		file_.setFileName(project.generic_string().c_str());
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();
		generatePatchFile();
	}

	void Project::generatePatchFile() {


		QJsonObject ro;
		ro.insert("AppName", gameName.c_str());
		if (version_.isEmpty()) {
			std::cout << "specify Patch version\n";
			return;
		}
		ro.insert("Ver", version_);

		QJsonArray deleted;
		for (auto& del : deleted_) {
			deleted.append(del);
		}
		ro.insert("RemovedFiles", deleted);

		QJsonArray changed;
		for (auto& del : changed_) {
			changed.append(del);
		}
		ro.insert("ChangedFiles", changed);

		QJsonArray added;
		for (auto& del : added_) {
			added.append(del);
		}
		ro.insert("AddedFiles", added);
		QJsonDocument d;
		d.setObject(ro);
		QFile file;
		auto path = projectDir / projectName / ("Patch-" + version_.toStdString() + ".json");
		file.setFileName(path.generic_string().c_str());
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(d.toJson());
		file.close();
	}

	void Project::insertData(dt::TreeItem* item, QJsonObject& object) {
		auto size = item->childCount();
		for (size_t i = 0; i < size; i++) {
			auto* child = item->child(i);
			std::cout << "file: " << child->path().toStdString() << std::endl;
			QJsonObject file;
			QJsonObject fileList;
			if (child->getState() != dt::TreeItem::fileState::DELETED) {
				if (child->isDirectory()) {
					insertData(child, file);
					fileList.insert("Size", QString::number(0));
					fileList.insert("Sha", "");
					fileList.insert("Id", QString::number(packetId));
				} else {
					file.insert("Size", QString::number(child->fileSize()));
					file.insert("Sha", child->fileSha());
					fileList.insert("Size", QString::number(child->fileSize()));
					fileList.insert("Sha", child->fileSha());
					fileList.insert("Id", QString::number(packetId));
				}
				auto fullPath = AppDir_ / child->path().toStdString();
				object.insert(fullPath.filename().c_str(), file);
				packer_.write(fullPath.generic_string(), child->path().toStdString(), child->isDirectory());
				rootObject_->insert(child->path(), fileList);
			}
			switch (child->getState()) {
			case dt::TreeItem::fileState::ADDED:
				added_.insert(child->path());
				break;
			case dt::TreeItem::fileState::DELETED:
				deleted_.insert(child->path());
				break;
			case dt::TreeItem::fileState::CHANGED:
				changed_.insert(child->path());
				break;
			case dt::TreeItem::fileState::SAME:
			default:
				break;
			}
		}
	}

	Q_INVOKABLE void Project::loadProject() {


		gameName = d["AppName"].toString().toStdString();
		version_ = d["Ver"].toString();

		projectName = d["ProjectName"].toString().toStdString();
		projectDir = d["ProjectDir"].toString().toStdString();

		auto& packets = dt::TreeModel::getObject().getPackets();
		QJsonObject AppComponents = d["AppComponents"].toObject();
		for (auto& it = AppComponents.begin(); it != AppComponents.end(); it++) {
			QJsonObject& pack = it->toObject();
			auto* newPacket = new dt::TreeModel(true); //packet
			newPacket->setPacketName(it.key());
			dt::TreeItem* item = dt::TreeModel::getObject().rootItemPtr();
			std::filesystem::path p;
			readPacket(newPacket->rootItemPtr(), pack["Files"].toObject(), item, p);
			packets.append(newPacket);
		}
		numberOFPacks = packets.size();
		nmbChanged();
	}

	namespace {
		void addDeleted(QJsonObject& object, dt::TreeItem* root, std::filesystem::path path) {
			for (auto& it = object.begin(); it != object.end(); it++) {
				QJsonObject& pack = it->toObject();
				if (!pack.isEmpty() && !pack.begin()->isObject()) { // for sure object is file
					auto size = std::stoll(pack["Size"].toString().toStdString());
					auto& sha = pack["Sha"].toString();
					auto* appended = root->appendChildren({ it.key(),  (path / it.key().toStdString()).generic_string().c_str() }, false, sha, size);
					appended->setState(dt::TreeItem::fileState::DELETED);
				} else {
					auto* appended = root->appendChildren({ it.key(),  (path / it.key().toStdString()).generic_string().c_str() }, true, "", 0);
					appended->setState(dt::TreeItem::fileState::DELETED);
					addDeleted(pack, appended, path / it.key().toStdString());
				}
			}
		}
	}

	void Project::readPacket(dt::TreeItem* item, QJsonObject& object, dt::TreeItem* root, std::filesystem::path path) {
		auto size = root->childCount();
		for (size_t i = 0; i < size; i++) {
			auto* child = root->child(i);
			if (!child->checked() && !child->isDirectory()) { // item is unchecked file
				auto fileName = child->fileName();
				auto& f = fileName.toStdString();
				auto it = object.find(fileName);
				if (it != object.end()) { // found object in packet
					QJsonObject& file = it->toObject();
					if (!file.isEmpty() && !file.begin()->isObject()) { // for sure object is file
						auto size = std::stoll(file["Size"].toString().toStdString());
						auto& sha = file["Sha"].toString();
						auto* appended = item->appendChildren({ it.key(),  child->path() }, false, sha, size);
						bool sizeCmp = size == child->fileSize(), shaCmp = sha == child->fileSha();
						if (!sizeCmp || !shaCmp) {
							appended->setState(dt::TreeItem::fileState::CHANGED);
						} else if (sizeCmp && shaCmp) {
							appended->setState(dt::TreeItem::fileState::SAME);
						} else {
							//problem
						}
						child->markRemove();
					} else {
						std::cout << "file: "; //todo
						throw std::exception();
					}
					child->check(); // mark as checked
					object.erase(it);
				} else { // not found = new file apered

				}

			} else if (child->isDirectory()) {
				auto fileName = child->fileName();
				auto& f = fileName.toStdString();
				auto it = object.find(fileName);
				if (it != object.end()) { // found object in packet					
					QJsonObject& file = it->toObject();
					if (!file.isEmpty()) {
						auto* appended = item->appendChildren({ it.key(),  child->path() }, true, "", 0);
						appended->setState(dt::TreeItem::fileState::SAME);
						readPacket(appended, file, child, path / fileName.toStdString());
					}
					object.erase(it);
					child->markRemove();
					child->check(); // mark as checked
				}
			}
		}
		addDeleted(object, item, path);
	}

	void Project::verify(dt::TreeItem* item) {
		int size = item->childCount();
		auto& map = dt::TreeModel::getObject().getSetUpModel().getDirFiles();
		auto& path = item->path();
		auto it = map.find(path);
		if (it == map.end()) { // new element
			if (item->isDirectory()) {
				auto full = AppDir_ / item->path().toStdString();
				if (!std::filesystem::exists(full))
					item->setState(dt::TreeItem::fileState::DELETED);
			} else { // file so muse be deleted
				item->setState(dt::TreeItem::fileState::DELETED);
			}
		} else {
			if (!it->dir && !item->isDirectory()) {
				bool sizeCmp = it->size == item->fileSize(), shaCmp = it->sha == item->fileSha();
				if (!sizeCmp || !shaCmp) {
					item->setState(dt::TreeItem::fileState::CHANGED);
				} else if (sizeCmp && shaCmp) {
					item->setState(dt::TreeItem::fileState::SAME);
				}
			} else {} // error
			auto& order = dt::TreeModel::getObject().getSetUpModel().getOrder();
			order.erase(it->it);
			map.erase(it);
		}
	}
}

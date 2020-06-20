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

		std::size_t filesInDir(std::filesystem::path path) {
			return (std::size_t)std::distance(std::filesystem::directory_iterator{ path }, std::filesystem::directory_iterator{});
		}
	}

	void Project::generate() {
		rootObject_ = doc_.object();
		rootObject_.insert("GameName", gameName.c_str());
		rootObject_.insert("GameDir", gameDir_.generic_string().c_str());
		rootObject_.insert("ProjectName", projectName.c_str());
		rootObject_.insert("ProjectDir", projectDir.generic_string().c_str());

		int index = 0;
		for (auto& p : fs::recursive_directory_iterator(gameDir_)) {
			if (p.is_directory())
				continue;
			insertFileData(p);
			index++;
		}
		doc_.setObject(rootObject_);
		file_.setFileName("fileList.json");
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();
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
		rootObject_.insert(relative.generic_string().c_str(), o);
	}

}

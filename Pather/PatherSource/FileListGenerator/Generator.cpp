#include "Generator.hpp"
#include <filesystem>
#include <iostream>
#include <QCryptographicHash>

namespace gn {
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

	void Generator::generate(std::filesystem::path dir) {
		rootDir_ = dir;
		rootObject_ = doc_.object();
		auto total = filesInDir(dir);
		int index = 0;
		for (auto& p : fs::recursive_directory_iterator(dir)) {
			if (p.is_directory())
				continue;
			std::cout << index << "/" << total << " : " << p.path() << '\n';
			insertFileData(p);
			index++;
		}
		doc_.setObject(rootObject_);
		file_.setFileName("fileList.json");
		file_.open(QIODevice::WriteOnly | QIODevice::Text);
		file_.write(doc_.toJson());
		file_.close();
	}
	void Generator::insertFileData(std::filesystem::path file) {
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

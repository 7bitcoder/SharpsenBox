#include "setUpTreeModel.hpp"
#include "TreeModel.hpp"
#include "Project.hpp"
#include <QCryptographichash>
#include <QFile>

namespace st {
	namespace {
		std::size_t filesInDir(std::filesystem::path path) {
			return (std::size_t)std::distance(std::filesystem::recursive_directory_iterator{ path }, std::filesystem::recursive_directory_iterator{});
		}
		std::pair<QString, qint64 > fileChecksum(QString fileName,
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

	void setUpModel::run() {
		try {
			actual_ = 1;
			total_ = filesInDir(dt::TreeModel::getRoot() / root_);
			if (load)
				loadData(root_);
			else
				setupModelData(root_, parent_);
			ended();
		} catch (std::exception& e) {
			error(e.what());
		} catch (...) {
			error("Unknown Error ocured");
		}
	}

	void setUpModel::setupModelData(const std::filesystem::path lines, dt::TreeItem* parent) {

		for (auto& path : std::filesystem::directory_iterator(dt::TreeModel::getRoot() / lines)) {
			auto p = std::filesystem::relative(path.path(), dt::TreeModel::getRoot());
			double per = actual_;
			per /= total_;
			stateChanged(per);
			actual_++;
			if (path.is_directory()) {
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, false, "" , 0);
				setupModelData(p, appended);
			} else {
				auto data = fileChecksum(path.path().generic_string().c_str(), QCryptographicHash::Algorithm::RealSha3_256);
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, false, data.first, data.second);
			}
		}
	}

	void setUpModel::loadData(const std::filesystem::path lines) {

		for (auto& path : std::filesystem::recursive_directory_iterator(dt::TreeModel::getRoot() / lines)) {
			auto p = std::filesystem::relative(path.path(), dt::TreeModel::getRoot());
			double per = actual_;
			per /= total_;
			stateChanged(per);
			actual_++;
			if (path.is_directory()) {
				dirFiles_.insert(p.generic_string().c_str(), { true, 0, "" });
			} else {
				auto data = fileChecksum(path.path().generic_string().c_str(), QCryptographicHash::Algorithm::RealSha3_256);
				dirFiles_.insert(p.generic_string().c_str(), { false, data.second, data.first });
			}
		}
		pr::Project::getObject().loadProject();
	}
}

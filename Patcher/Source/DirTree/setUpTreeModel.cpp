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
			std::cout << e.what();
			error(e.what());
		} catch (...) {
			std::cout << "Unknown Error ocured";
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
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, true, "", 0);
				setupModelData(p, appended);
			} else {
				auto data = fileChecksum(path.path().generic_string().c_str(), QCryptographicHash::Algorithm::RealSha3_256);
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, false, data.first, data.second);
			}
		}
	}

	void setUpModel::setupModelData(QLinkedList<File*>::reverse_iterator& it, dt::TreeItem* parent, const std::filesystem::path& parentPath) {

		for (; it != order_.rend(); it++) {
			auto& path = *it;
			std::filesystem::path p = path->path.toStdString();
			auto& par = p.parent_path();
			if (par != parentPath && par.parent_path() != parentPath)
				return;
			if (path->dir) {
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, true, "", 0);
				appended->setState(dt::TreeItem::fileState::ADDED);
				setupModelData(++it, appended, p);
			} else {
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, false, path->sha, path->size);
				appended->setState(dt::TreeItem::fileState::ADDED);
			}
			if (it == order_.rend())
				break;
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
				auto it = dirFiles_.insert(p.generic_string().c_str(), { p.generic_string().c_str(), true, 0, "" });
				order_.push_front(&(*it));
				it->it = order_.begin();
			} else {
				auto data = fileChecksum(path.path().generic_string().c_str(), QCryptographicHash::Algorithm::RealSha3_256);
				auto it = dirFiles_.insert(p.generic_string().c_str(), { p.generic_string().c_str(), false, data.second, data.first });
				order_.push_front(&(*it));
				it->it = order_.begin();
			}
		}
		pr::Project::getObject().loadProject();
		setupLoaded();
	}
}

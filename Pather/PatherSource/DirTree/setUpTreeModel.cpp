#include "setUpTreeModel.hpp"
#include "TreeModel.hpp"
#include "Project.hpp"

namespace st {
	namespace {
		std::size_t filesInDir(std::filesystem::path path) {
			return (std::size_t)std::distance(std::filesystem::recursive_directory_iterator{ path }, std::filesystem::recursive_directory_iterator{});
		}
	}

	void setUpModel::run() {
		try {
			actual_ = 1;
			total_ = filesInDir(dt::TreeModel::getRoot() / root_);
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
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, true);
				setupModelData(p, appended);
			} else {
				auto* appended = parent->appendChildren({ p.filename().generic_string().c_str(),  p.generic_string().c_str() }, false);
			}
		}
	}
}

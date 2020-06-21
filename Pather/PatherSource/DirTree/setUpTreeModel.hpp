#pragma once
#include <QThread>
#include <QObject>
#include <filesystem>
#include "TreeItem.hpp"

namespace st {
	class setUpModel : public QThread {
		Q_OBJECT
	public:

		void run() override;

		void setupModelData(const std::filesystem::path, dt::TreeItem* parent);
		void setRoot(std::filesystem::path root) { root_ = root; }
		void setParent(dt::TreeItem* parent) { parent_ = parent; }
	signals:
		void stateChanged(double percent);
		void ended();
		void error(QString err);

	private:
		size_t total_;
		size_t actual_;
		std::filesystem::path root_;
		dt::TreeItem* parent_;
	};
}
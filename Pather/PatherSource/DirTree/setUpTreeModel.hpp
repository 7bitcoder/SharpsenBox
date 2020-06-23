#pragma once
#include <QThread>
#include <QObject>
#include <filesystem>
#include "TreeItem.hpp"

namespace st {
	struct File {
		bool dir;
		qint64 size;
		QString sha;
	};
	class setUpModel : public QThread {
		Q_OBJECT
	public:

		void run() override;

		void setupModelData(const std::filesystem::path, dt::TreeItem* parent);
		void setRoot(std::filesystem::path root) { root_ = root; }
		void setParent(dt::TreeItem* parent) { parent_ = parent; }
		void loadData(const std::filesystem::path lines);
		QHash<QString, File>& getDirFiles() { return dirFiles_; }
	signals:
		void stateChanged(double percent);
		void ended();
		void error(QString err);

	private:
		size_t total_;
		size_t actual_;
		std::filesystem::path root_;
		dt::TreeItem* parent_;

		QHash<QString, File> dirFiles_;
		bool load = false;
	};
}
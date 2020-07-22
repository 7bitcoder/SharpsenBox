#pragma once
#include <QThread>
#include <QObject>
#include <QLinkedList>
#include <filesystem>
#include "TreeItem.hpp"

namespace st {
	struct File {
		QString path;
		bool dir;
		qint64 size;
		QString sha;
		QLinkedList<File*>::iterator it;
	};
	class setUpModel : public QThread {
		Q_OBJECT
	public:
		void run() override;
		void setupModelData(const std::filesystem::path, dt::TreeItem* parent);
		void setupLoaded() {
			g = order_.size();
			setupModelDataAfterLoad(parent_, "");
		}
		void setupModelDataAfterLoad(dt::TreeItem* parent, const  std::filesystem::path& parentPath);
		void setRoot(std::filesystem::path root) { root_ = root; }
		void setParent(dt::TreeItem* parent) { parent_ = parent; }
		void loadData(const std::filesystem::path lines);
		QHash<QString, File>& getDirFiles() { return dirFiles_; }
		QLinkedList<File*>& getOrder() { return order_; }
		void setLoad(bool val) { load = true; }
	signals:
		void stateChanged(double percent);
		void ended();
		void error(QString err);

	private:
		int g = 0;
		size_t total_;
		size_t actual_;
		std::filesystem::path root_;
		dt::TreeItem* parent_;

		QHash<QString, File> dirFiles_;
		QLinkedList<File*> order_;
		bool load = false;
	};
}

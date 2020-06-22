#include <iostream>
#include <string>
#include "TreeItem.hpp"
#include <filesystem>
#include <QCryptographichash>
#include <QFile>
#include "Project.hpp"
#include "TreeModel.hpp"

namespace dt {
	namespace fs = std::filesystem;
	namespace {
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

		std::size_t filesInDir(std::filesystem::path path) {
			return (std::size_t)std::distance(std::filesystem::directory_iterator{ path }, std::filesystem::directory_iterator{});
		}
	}

	TreeItem::TreeItem(const QVector<QVariant>& data, bool isDIr, QString& sha_, qint64 size_, TreeItem* parent)
		: itemData(data),
		parentItem(parent) {
		QString path = data[1].toString();
		auto& str = path.toStdString();
		isDir = isDIr;
		state = fileState::SAME;
		sha = sha_;
		size = size_;
	}

	TreeItem::~TreeItem() {
		qDeleteAll(childItems);
	}

	TreeItem* TreeItem::parent() {
		return parentItem;
	}

	TreeItem* TreeItem::child(int number) {
		if (number < 0 || number >= childItems.size())
			return nullptr;
		return childItems.at(number);
	}

	int TreeItem::childCount() const {
		return childItems.count();
	}

	int TreeItem::childNumber() const {
		if (parentItem)
			return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
		return 0;
	}

	int TreeItem::columnCount() const {
		return itemData.count();
	}

	QVariant TreeItem::data(int column) const {
		if (column < 0 || column >= itemData.size())
			return QVariant();
		return itemData.at(column);
	}

	bool TreeItem::setData(int column, const QVariant& value) {
		if (column < 0 || column >= itemData.size())
			return false;

		itemData[column] = value;
		return true;
	}

	TreeItem* TreeItem::appendChildren(QVector<QVariant> data, bool isDir, const QString& sha_, qint64 size_) {
		TreeItem* item = new TreeItem(data, isDir, sha, size, this);
		childItems.append(item);
		return item;
	}

	TreeItem* TreeItem::appendChildren(TreeItem* item) {
		item->parentItem = this;
		childItems.append(item);
		return item;
	}

	//bool TreeItem::insertChildren(int position, int count, int columns) {
	//    if (position < 0 || position > childItems.size())
	//        return false;
	//
	//    for (int row = 0; row < count; ++row) {
	//        QVector<QVariant> data(columns);
	//        TreeItem* item = new TreeItem(data, this);
	//        childItems.insert(position, item);
	//    }
	//
	//    return true;
	//}

	bool TreeItem::removeChildren(int position, int count) {
		if (position < 0 || position + count > childItems.size())
			return false;

		for (int row = 0; row < count; ++row)
			delete childItems.takeAt(position);

		return true;
	}

	TreeItem* TreeItem::unbindChildren(int position) {
		if (position < 0 || position > childItems.size())
			return nullptr;

		auto* item = child(position);
		if (!item)
			return item;
		item->parentItem = nullptr;
		childItems.remove(position);

		return item;
	}

	bool TreeItem::insertColumns(int position, int columns) {
		if (position < 0 || position > itemData.size())
			return false;

		for (int column = 0; column < columns; ++column)
			itemData.insert(position, QVariant());

		for (TreeItem* child : qAsConst(childItems))
			child->insertColumns(position, columns);

		return true;
	}
}

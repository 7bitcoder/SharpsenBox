#include <iostream>
#include <string>
#include "TreeItem.hpp"

namespace dt {

	TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
		: m_itemData(data), m_parentItem(parent) {}

	TreeItem::~TreeItem() {
		qDeleteAll(m_childItems);
	}

	void TreeItem::appendChild(TreeItem* item) {
		m_childItems.append(item);
	}

	TreeItem* TreeItem::child(int row) {
		if (row < 0 || row >= m_childItems.size())
			return nullptr;
		return m_childItems.at(row);
	}

	int TreeItem::childCount() const {
		return m_childItems.count();
	}

	int TreeItem::row() const {
		if (m_parentItem)
			return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

		return 0;
	}

	int TreeItem::columnCount() const {
		return m_itemData.count();
	}

	QVariant TreeItem::data(int column) const {
		if (column < 0 || column >= m_itemData.size())
			return QVariant();
		return m_itemData.at(column);
	}

	TreeItem* TreeItem::parentItem() {
		return m_parentItem;
	}
}

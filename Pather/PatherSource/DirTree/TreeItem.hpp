#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include "IQmlObject.hpp"
#include <QVector>

namespace dt {
	class TreeItem {
	public:
		explicit TreeItem(const QVector<QVariant>& data, TreeItem* parentItem = nullptr);
		~TreeItem();

		void appendChild(TreeItem* child);

		TreeItem* child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		TreeItem* parentItem();

	private:
		QVector<TreeItem*> m_childItems;
		QVector<QVariant> m_itemData;
		TreeItem* m_parentItem;
	};
}

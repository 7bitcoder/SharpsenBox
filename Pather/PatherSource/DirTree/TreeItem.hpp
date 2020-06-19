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
        explicit TreeItem(const QVector<QVariant>& data, TreeItem* parent = nullptr);
        ~TreeItem();

        TreeItem* child(int number);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        TreeItem* appendChildren(QVector<QVariant> data);
        TreeItem* appendChildren(TreeItem* item);
        bool insertChildren(int position, int count, int columns);
        bool insertColumns(int position, int columns);
        TreeItem* parent();
        bool removeChildren(int position, int count);
        TreeItem* unbindChildren(int position);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        bool setData(int column, const QVariant& value);

    private:
        QVector<TreeItem*> childItems;
        QVector<QVariant> itemData;
        TreeItem* parentItem;
    };
}

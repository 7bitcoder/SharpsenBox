#include <QDir>
#include <QDirIterator>
#include "TreeModel.hpp"

namespace dt {
    TreeModel::TreeModel(const QStringList& headers, const QString& data, QObject* parent)
        : QAbstractItemModel(parent) {
        QVector<QVariant> rootData;
        for (const QString& header : headers)
            rootData << header;

        rootItem = new TreeItem(rootData);
        setupModelData(".", rootItem);
    }

    TreeModel::~TreeModel() {
        delete rootItem;
    }

    TreeItem* TreeModel::getItem(const QModelIndex& index) const {
        if (index.isValid()) {
            TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
            if (item)
                return item;
        }
        return rootItem;
    }

    int TreeModel::rowCount(const QModelIndex& parent) const {
        const TreeItem* parentItem = getItem(parent);

        return parentItem ? parentItem->childCount() : 0;
    }

    int TreeModel::columnCount(const QModelIndex& parent) const {
        Q_UNUSED(parent);
        return rootItem->columnCount();
    }

    Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const {
        if (parent.isValid() && parent.column() != 0)
            return QModelIndex();
       
            TreeItem* parentItem = getItem(parent);
        if (!parentItem)
            return QModelIndex();

        TreeItem* childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
    }

    QModelIndex TreeModel::parent(const QModelIndex& index) const {
        if (!index.isValid())
            return QModelIndex();

        TreeItem* childItem = getItem(index);
        TreeItem* parentItem = childItem ? childItem->parent() : nullptr;

        if (parentItem == rootItem || !parentItem)
            return QModelIndex();

        return createIndex(parentItem->childNumber(), 0, parentItem);
    }

    void TreeModel::setupModelData(const std::filesystem::path lines, TreeItem* parent) {
        for (auto& p : std::filesystem::directory_iterator(lines)) {
            if (p.is_directory()) {
                auto* appended = parent->appendChildren({ p.path().filename().generic_string().c_str(),  p.path().generic_string().c_str() });
                setupModelData(p, appended);
            } else {
                auto* appended = parent->appendChildren({ p.path().filename().generic_string().c_str(),  p.path().generic_string().c_str() });
            }
        }
    }
}

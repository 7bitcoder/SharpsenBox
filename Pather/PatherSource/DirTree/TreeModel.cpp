#include <QDir>
#include <QDirIterator>
#include "TreeModel.hpp"

namespace dt {
    TreeModel::TreeModel(const std::filesystem::path& rootPath, QObject* parent)
        : QAbstractItemModel(parent) {
        QVector<QVariant> rootData;
        rootData << rootPath.generic_string().c_str();

        rootItem = new TreeItem(rootData);
        auto * ptr = rootItem->appendChildren({rootPath.generic_string().c_str(), rootPath.generic_string().c_str()});
        setupModelData(rootPath, ptr);
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


    QVariant TreeModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole)
            return QVariant();

        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

        return item->data(index.column());
    }


    QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return rootItem->data(section);

        return QVariant();
    }

    bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
        const QVariant& value, int role ) {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            rootItem->setData(section, value);
            return true;
        }

        return false;
    }

    QModelIndex TreeModel::unbindRows(int position, int rows, const QModelIndex& curr) {
        if (position != -1 && rows != -1) {
            beginRemoveRows(curr.parent(), curr.row(), curr.row());

            auto* item = getItem(curr)->parent();
            auto* unbinded = item->unbindChildren(curr.row());
            endRemoveRows();
            return createIndex(0, 0, unbinded);
        } else {
            return QModelIndex();
        }
    }

    Q_INVOKABLE void TreeModel::remove(const QModelIndex& index) {
        auto* item = getItem(index);
       // beginRemoveRows(QModelIndex(), position, position + rows - 1);
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

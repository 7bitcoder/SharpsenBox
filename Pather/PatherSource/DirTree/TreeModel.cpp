#include <QDir>
#include <QDirIterator>
#include "TreeModel.hpp"
#include <QMimeData>
#include <istream>
#include <iostream>
#include <sstream>

namespace dt {
    namespace {
        std::size_t filesInDir(std::filesystem::path path) {
            return (std::size_t)std::distance(std::filesystem::recursive_directory_iterator{ path }, std::filesystem::recursive_directory_iterator{});
        }
    }

    std::filesystem::path TreeModel::rootDir_;
    TreeModel::TreeModel(bool packet, QObject* parent)
        : QAbstractItemModel(parent) {
        if (packet)
            init(packet);
    }

    void TreeModel::init(bool packet) {
        QVector<QVariant> rootData;
        rootData << rootDir_.generic_string().c_str() << rootDir_.generic_string().c_str();
        rootItem = new TreeItem(rootData, true);
        auto* ptr = rootItem->appendChildren({ rootDir_.generic_string().c_str(), rootDir_.generic_string().c_str() }, true);
        if (!packet) {
            beginResetModel();
            connect(&setUp_, &st::setUpModel::ended, this, &TreeModel::readEnded);
            connect(&setUp_, &st::setUpModel::stateChanged, this, &TreeModel::readState);
            setUp_.setParent(ptr);
            setUp_.setRoot(".");
            setUp_.start();
        }
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

    QModelIndex TreeModel::unbindRows(const QModelIndexList& list) {
        for (auto& curr : list) {
            beginRemoveRows(curr.parent(), curr.row(), curr.row());

            auto* item = getItem(curr)->parent();
            auto* unbinded = item->unbindChildren(curr.row());
            endRemoveRows();
            return createIndex(0, 0, unbinded);
        }
    }

    Q_INVOKABLE void TreeModel::remove(const QModelIndexList& list) {
        ///auto* item = getItem(index);
       // beginRemoveRows(QModelIndex(), position, position + rows - 1);
    }


    Q_INVOKABLE void  TreeModel::setSelected(const QModelIndex& index) {
        selectedToMove_.insert(index);
    }

    Q_INVOKABLE void TreeModel::unsetSelected(const QModelIndex& index) {
        selectedToMove_.remove(index);
    }


    QMimeData* TreeModel::mimeData(const QModelIndexList& indexes) const {
        QMimeData* mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        foreach(QModelIndex index, indexes) {
            if (index.isValid()) {
                QString text = data(index, Qt::DisplayRole).toString();
                stream << text;
            }
        }

        mimeData->setData("application/vnd.text.list", encodedData);
        return mimeData;
    }

    Q_INVOKABLE void TreeModel::bind(const QModelIndexList& list) {
       //modelAboutToBeReset();
        beginResetModel();
        for (auto& path : list) {
            bind(path);
        }
        endResetModel();

       // modelReset();
    }

    namespace {
        std::string p(TreeItem* item) {
            return item->data(0).toString().toStdString();
        }
    }

    void TreeModel::addData(TreeItem* parent) {
        int size = parent->childCount();
        bool found = false;
        if (dataToInsert_.depth >= dataToInsert_.folders.size())
            return;
        auto& element = dataToInsert_.folders.at(dataToInsert_.depth);
        for (int i = 0; i < size; i++) {
            auto& p2 = p(parent->child(i));
            if ( element == p2 ) {
                if (dataToInsert_.depth == dataToInsert_.folders.size() - 1) {//last element
                    merge(parent->child(i), dataToInsert_.item);
                    return;
                } else {
                    dataToInsert_.depth++;
                    addData(parent->child(i));
                }
                found = true;
                break;
            }
        } if (!found) {
            if (dataToInsert_.depth == dataToInsert_.folders.size() - 1) //last element
                parent->appendChildren(dataToInsert_.item);
            else {

                auto p = rootDir_;
                for (int j = 0; j <= dataToInsert_.depth; ++j) {
                    p /= dataToInsert_.folders.at(j);
                }
                parent->appendChildren({ element.c_str(), p.generic_string().c_str() }, std::filesystem::is_directory(p));
                dataToInsert_.depth++;
                addData(parent->child(parent->childCount() - 1));// last element
            }
        }
    }
    void TreeModel::merge(TreeItem* parent, TreeItem* toInsert) {
        int size = toInsert->childCount();
        bool found = false;
        for (int i = 0; i < size; i++) {
            auto* child = toInsert->child(i);
            if (!child->childCount()) {//file
                parent->appendChildren(child);
            } else { //dir
                int sizeP = parent->childCount();
                std::string childS = p(child);
                for (int j = 0; j < sizeP; ++j) {
                    if (childS == p(parent->child(j))) {
                        merge(parent->child(j), child);
                        found = true;
                    }
                } if (!found) {
                    parent->appendChildren(child);
                }
            }
        } if (found)
            delete toInsert;
    }

    void TreeModel::bind(const QModelIndex& index) {

        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        dataToInsert_.item = item;
        dataToInsert_.path = item->data(1).toString().toStdString();
        dataToInsert_.dir = std::filesystem::is_directory(dataToInsert_.path);
        dataToInsert_.folders.clear();
        dataToInsert_.depth = 0;
        std::stringstream relStr(dataToInsert_.path);
        std::string segment;
        while (std::getline(relStr, segment, '/')) {
            dataToInsert_.folders.push_back(segment);
        }
        addData(rootItem->child(0));
    }

    void TreeModel::setupModelData(const std::filesystem::path lines, TreeItem* parent) {
        
        for (auto& path : std::filesystem::directory_iterator(rootDir_ / lines)) {
            auto p = std::filesystem::relative(path.path(), rootDir_);
            std::cout << actual_ << "/" << total_ << std::endl;
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

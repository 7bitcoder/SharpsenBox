#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QAbstractItemModel>
#include <QObject>
#include <QDir>
#include <QStandardItemModel>
#include <QStandardItem>
#include "IQmlObject.hpp"
#include "TreeItem.hpp"
#include <iostream>
#include <filesystem>

namespace dt {
    class TreeModel : public QAbstractItemModel {
        Q_OBJECT

    public:

        // implementation IQmlObject
        void update()  {};
        std::string getName() {
            return TYPENAME(TreeModel);
        }
        void init()  {};

        Q_INVOKABLE void remove(const QModelIndex& index);


        TreeModel(const std::filesystem::path& rootPath, QObject* parent = nullptr);
        ~TreeModel();

        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;

        QModelIndex index(int row, int column,
            const QModelIndex& parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex& index) const override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;
       // bool setData(const QModelIndex& index, const QVariant& value,
       //     int role = Qt::EditRole) override;
        bool setHeaderData(int section, Qt::Orientation orientation,
            const QVariant& value, int role = Qt::EditRole) override;

      //  bool insertColumns(int position, int columns,
      //      const QModelIndex& parent = QModelIndex()) override;
        //bool removeColumns(int position, int columns,
       //     const QModelIndex& parent = QModelIndex()) override;
     //   bool insertRows(int position, int rows,
      //      const QModelIndex& parent = QModelIndex()) override;
        Q_INVOKABLE QModelIndex unbindRows(int position, int rows,
           const QModelIndex& parent = QModelIndex());

    private:
        void setupModelData(const std::filesystem::path, TreeItem* parent);
        TreeItem* getItem(const QModelIndex& index) const;

        TreeItem* rootItem;
    };
}

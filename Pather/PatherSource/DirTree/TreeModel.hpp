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
#include <QHash>
#include "setUpTreeModel.hpp"

namespace dt {
    class TreeModel : public QAbstractItemModel {
        Q_OBJECT

    public:
        static TreeModel& getObject() {
            static TreeModel uc(false);
            return uc;
        }
        static int index_;
        // implementation IQmlObject
        void update()  {};
        std::string getName() {
            return TYPENAME(TreeModel);
        }
        void init(bool packet);

        // Qml properties
        Q_PROPERTY(QString packetName WRITE setPacketName READ getPacketName);

        Q_INVOKABLE void setPacketName(QString str) {
            packetName_ = str.toStdString();
        }

        Q_INVOKABLE QString getPacketName() {
            return packetName_.c_str();
        }

        Q_PROPERTY(double percentage READ getPercent NOTIFY percentChanged);
        Q_PROPERTY(bool available READ getAval NOTIFY avalChanged);

        Q_INVOKABLE double getPercent() {
            return percentage_;
        }

        Q_INVOKABLE double getAval() {
            return available_;
        }

        TreeModel(bool packet = true, QObject* parent = nullptr);
        ~TreeModel();

        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;

        QModelIndex index(int row, int column,
            const QModelIndex& parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex& index) const override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        TreeItem* item(const QModelIndex& index) {return  static_cast<TreeItem*>(index.internalPointer());}
        Qt::ItemFlags flags(const QModelIndex& index) const override;
       // bool setData(const QModelIndex& index, const QVariant& value,
       //     int role = Qt::EditRole) override;
        bool setHeaderData(int section, Qt::Orientation orientation,
            const QVariant& value, int role = Qt::EditRole) override;

      //  bool insertColumns(int position, int columns,
      //      const QModelIndex& parent = QModelIndex()) override;
      // bool removeColumns(int position, int columns,
         //   const QModelIndex& parent = QModelIndex()) override;
       //   bool insertRows(int position, int rows,
      //      const QModelIndex& parent = QModelIndex()) override;
        Q_INVOKABLE QModelIndex unbindRows(const QModelIndexList& list);
        Q_INVOKABLE void setSelected(const QModelIndex& index);
        Q_INVOKABLE void unsetSelected(const QModelIndex& index);

        Q_INVOKABLE void print(const QModelIndexList list) {
                for (int i = 0; i < list.size(); i++) {
                    std::cout << list[i].data().toString().toStdString() <<std::endl;
                }
        }

        void bind(const QModelIndex& list);

        Q_INVOKABLE void remove(const QModelIndexList& list);
        Q_INVOKABLE QAbstractItemModel* getNewPacket() { auto* ptr = new TreeModel(); packets.push_back(ptr); return packets.back(); }

        Q_INVOKABLE int getFileState(const QModelIndex& index) {
            if(index.isValid())
                return item(index)->getState();
            return 1;
        }

        static void setRoot(std::filesystem::path path) { rootDir_ = path; }
        static std::filesystem::path& getRoot() { return rootDir_ ; }
        QMimeData* mimeData(const QModelIndexList& indexes) const override;

    QVector<TreeModel*>& getPackets() { return packets; }

    signals:
        void percentChanged();
        void avalChanged();
    public slots:
        void bind(const QModelIndexList& list);
        void errorChatched(QString err) {};
        void readEnded() { endResetModel(); available_ = true; avalChanged(); }
        void readState(double percentage) { percentage_ = percentage; percentChanged(); }
    private:
        struct data {
            TreeItem* item;
            std::string path;
            bool dir;
            std::vector<std::string> folders;
            int depth;
        } dataToInsert_;

        double percentage_;
        void merge(TreeItem* parent, TreeItem* toInsert);
        QModelIndex* rootIndex_;
        void addData(TreeItem* parent);
        QSet<QModelIndex> selectedToMove_;
        void setupModelData(const std::filesystem::path, TreeItem* parent);
        TreeItem* getItem(const QModelIndex& index) const;
        static std::filesystem::path rootDir_;
        TreeItem* rootItem;
        QVector<TreeModel*> packets;
        size_t total_;
        size_t actual_ = 0;

        st::setUpModel setUp_;
        bool available_ = false;
        std::string packetName_ = std::string("packet") + std::to_string(index_++) + ".zip";
    };
}

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

namespace dt {
	class TreeModel : public QStandardItemModel {
		Q_OBJECT

	public:
		static TreeModel& getObject() {
			static TreeModel uc;
			uc.setSandBoxDetails(QDir::currentPath());
			return uc;
		}

		void update() {};
		std::string getName() { return TYPENAME(TreeModel); };
		void init() {};
		TreeModel(QObject* parent = 0);
		~TreeModel();

		void setSandBoxDetails(QString names);
		void populateSandBoxes(const QStringList& names);
		void createDirectoryItem(QString dirName, QStandardItem* parentItem = NULL);

	private:
		QStandardItem* rootItem;
		QIcon dirIcon;
		QIcon fileIcon;
	};
}

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
		enum fileState { CHANGED, DELETED, ADDED, SAME };
		explicit TreeItem(const QVector<QVariant>& data, bool isDIr, const  QString& sha_, qint64 size_, TreeItem* parent = nullptr);
		~TreeItem();

		int getState() { return state; }
		void setState(fileState st) { state = st; }

		TreeItem* child(int number);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		TreeItem* appendChildren(QVector<QVariant> data, bool isDir, const  QString& sha_, qint64 size_);
		TreeItem* appendChildren(TreeItem* item);
		//bool insertChildren(int position, int count, int columns);
		bool insertColumns(int position, int columns);
		TreeItem* parent();
		bool removeChildren(int position, int count);
		TreeItem* unbindChildren(int position);
		bool removeColumns(int position, int columns);
		int childNumber() const;
		bool setData(int column, const QVariant& value);

		qint64 fileSize() { return size; }
		QString& fileSha() { return sha; }
		QString fileName() { return data(0).toString(); }
		QString path() { return data(1).toString(); }
		bool isDirectory() { return isDir; }
		void markRemove() { toRemove = true; }
		void check() { checked_ = true; }
		bool checked() { return checked_; }
	private:
		fileState state;
		qint64 size = 0;
		QString sha;
		bool isDir;
		QVector<TreeItem*> childItems;
		QVector<QVariant> itemData;
		TreeItem* parentItem;
		bool toRemove = false;
		bool checked_ = false;
	};
}

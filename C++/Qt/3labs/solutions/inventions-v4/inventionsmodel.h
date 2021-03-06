#ifndef INVENTIONSMODEL_H
#define INVENTIONSMODEL_H

#include "invention.h"

#include <QAbstractItemModel>

class InventionsModel : public QAbstractTableModel {
	Q_OBJECT

public:
	InventionsModel(QObject* parent = nullptr);

	QVariant data(const QModelIndex &index, int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;
	//QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	//QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	enum { MaxColumnCount = 2 };
	// база данных открытий/изобретений
	QList<Invention> inventions;
};

#endif // INVENTIONSMODEL_H

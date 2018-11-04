#ifndef REGISTERDATAMODEL_H
#define REGISTERDATAMODEL_H

#include <QAbstractTableModel>

#include "testslavedata.h"

class RegisterDataModel : public QAbstractTableModel
{

public:
    explicit RegisterDataModel(TestSlaveData *pTestSlaveData, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex & index) const;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

protected:
    TestSlaveData *_pTestSlaveData;

    const quint16 _cRegistersPerRow = 10;

};

#endif // REGISTERDATAMODEL_H

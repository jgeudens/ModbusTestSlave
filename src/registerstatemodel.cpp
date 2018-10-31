#include "registerstatemodel.h"

RegisterStateModel::RegisterStateModel(TestSlaveData *pTestSlaveData, QObject *parent) : QAbstractTableModel (parent)
{
    _pTestSlaveData = pTestSlaveData;
}

QVariant RegisterStateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            return QVariant((section * _cRegistersPerRow) + 40001);
        }
        else
        {
            return QVariant(QString("%0").arg(section));
        }
    }

    return QVariant();
}

int RegisterStateModel::rowCount(const QModelIndex & /*parent*/) const
{
    if (_pTestSlaveData->size() <= _cRegistersPerRow)
    {
        return 1;
    }
    else
    {
        return _pTestSlaveData->size() / _cRegistersPerRow;
    }
}

int RegisterStateModel::columnCount(const QModelIndex & /*parent*/) const
{
    return _cRegistersPerRow;
}

QVariant RegisterStateModel::data(const QModelIndex &index, int role) const
{
    const int registerAddress = index.row() * _cRegistersPerRow + index.column();
    if (role == Qt::ToolTipRole)
    {
        if (registerAddress < _pTestSlaveData->size())
        {
            return QString("%0").arg(registerAddress + 40001);
        }
    }
    else if (role == Qt::CheckStateRole)
    {
        if (_pTestSlaveData->registerState(registerAddress))
        {
            return Qt::Checked;
        }
        else
        {
            return Qt::Unchecked;
        }
    }

    return QVariant();
}

Qt::ItemFlags RegisterStateModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::NoItemFlags;

    const int registerAddress = index.row() * _cRegistersPerRow + index.column();

    if (registerAddress < _pTestSlaveData->size())
    {
        flags |= Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool RegisterStateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool bRet = true;

    if (role == Qt::CheckStateRole)
    {
        const int registerAddress = index.row() * _cRegistersPerRow + index.column();

        _pTestSlaveData->setRegisterState(registerAddress, value == Qt::Checked);
    }

    // Notify view(s) of change
    emit dataChanged(index, index);

    return bRet;
}

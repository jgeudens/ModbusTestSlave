#include "registerdatamodel.h"
#include <QMessageBox>

RegisterDataModel::RegisterDataModel(TestSlaveData *pTestSlaveData, QObject *parent) : QAbstractTableModel (parent)
{
    _pTestSlaveData = pTestSlaveData;
}

QVariant RegisterDataModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int RegisterDataModel::rowCount(const QModelIndex & /*parent*/) const
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

int RegisterDataModel::columnCount(const QModelIndex & /*parent*/) const
{
    return _cRegistersPerRow;
}

QVariant RegisterDataModel::data(const QModelIndex &index, int role) const
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
    else if ((role == Qt::EditRole) || (role == Qt::DisplayRole))
    {
        return _pTestSlaveData->registerValue(registerAddress);
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags RegisterDataModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::NoItemFlags;

    const int registerAddress = index.row() * _cRegistersPerRow + index.column();

    if (registerAddress < _pTestSlaveData->size())
    {
        flags |= Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    }

    return flags;
}

bool RegisterDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool bRet = true;
    const int registerAddress = index.row() * _cRegistersPerRow + index.column();

    if (role == Qt::CheckStateRole)
    {
        _pTestSlaveData->setRegisterState(registerAddress, value == Qt::Checked);
    }
    else if (role == Qt::EditRole)
    {
        if (value.canConvert(QMetaType::UInt))
        {
            _pTestSlaveData->setRegisterValue(registerAddress, value.toUInt());
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("ModbusServer"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Register address is not a valid address between 40001 and 49999."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();

            bRet = false;
        }
    }
    else
    {
        bRet = false;
    }

    // Notify view(s) of change
    if (bRet)
    {
        emit dataChanged(index, index);
    }

    return bRet;
}

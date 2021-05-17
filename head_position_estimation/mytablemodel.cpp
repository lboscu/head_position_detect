#include "mytablemodel.h"
#include<QDebug>

MyTableModel::MyTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

MyTableModel::MyTableModel(int rows,int cols)
{
    this->setColumnCount(cols);
    this->setRowCount(rows);
    _rows = rows;
    _cols = cols;

    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<cols;j++)
        {
            qlstItem.push_back(new QStandardItem());
            this->setItem(i,j,qlstItem.at(i*cols+j));
        }
    }
}

void MyTableModel::setHeader(QString* qs,int len)
{
    for (int i=0;i<len;i++)
    {
        qlstHeader.push_back(new QStandardItem(qs[i]));
        this->setHorizontalHeaderItem(i,qlstHeader[i]);
    }
}

void MyTableModel::setHeader(int index, QString qs)
{
    if (index >=0 && index < qlstHeader.size())
    {
        qlstHeader.at(index)->setText(qs);
    }
}

void MyTableModel::setItemT(int row,int col, QString qs)
{
    if (row < 0 || row > _rows-1)
        return;
    if (col < 0 || col > _cols-1)
        return;
    qlstItem.at(row*_cols+col)->setText(qs);
}

void MyTableModel::clearItemT()
{
    for (int i=0;i<qlstItem.size();i++)
    {
        qlstItem.at(i)->setText("");
    }
}

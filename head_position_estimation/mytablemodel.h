#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include<QStandardItemModel>
#include<QList>

class MyTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MyTableModel(QObject *parent = nullptr);
    MyTableModel(int rows,int cols);

    void setHeader(QString* qs,int len);
    void setHeader(int index, QString qs);

    void setItemT(int row,int col, QString qs);
    void clearItemT();

private:
    QList<QStandardItem*> qlstHeader;
    QList<QStandardItem*> qlstItem;

    int _rows;
    int _cols;

signals:

};

#endif // MYTABLEMODEL_H

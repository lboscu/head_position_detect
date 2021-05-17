#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QWidget>
#include<QDockWidget>

namespace Ui {
class MyComboBox;
}

class MyComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit MyComboBox(QWidget *parent = nullptr);
    ~MyComboBox();

    void addItem(QString,QWidget*);

private:
    Ui::MyComboBox *ui;
};

#endif // MYCOMBOBOX_H

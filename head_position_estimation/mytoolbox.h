#ifndef MYTOOLBOX_H
#define MYTOOLBOX_H

#include <QWidget>

namespace Ui {
class MyToolBox;
}

class MyToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit MyToolBox(QWidget *parent = nullptr);
    ~MyToolBox();

private:
    Ui::MyToolBox *ui;
};

#endif // MYTOOLBOX_H

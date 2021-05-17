#ifndef MYCOMBOPAGE_H
#define MYCOMBOPAGE_H

#include <QWidget>

namespace Ui {
class MyComboPage;
}

class MyComboPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyComboPage(QWidget *parent = nullptr);
    ~MyComboPage();
    void setText(QString);
    void setWidget(QWidget*);

private:
    Ui::MyComboPage *ui;
};

#endif // MYCOMBOPAGE_H

#include "mycombobox.h"
#include "ui_mycombobox.h"
#include "mycombopage.h"

MyComboBox::MyComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyComboBox)
{
    ui->setupUi(this);
}

MyComboBox::~MyComboBox()
{
    delete ui;
}


void MyComboBox::addItem(QString qs,QWidget* qw)
{
    MyComboPage* page = new MyComboPage;

    int num = ui->verticalLayout->count();
    assert(num>0);
    ui->verticalLayout->insertWidget(num-1,page);

    page->setText(qs);
    page->setWidget(qw);
}

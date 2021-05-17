#include "mytoolbox.h"
#include "ui_mytoolbox.h"

MyToolBox::MyToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyToolBox)
{
    ui->setupUi(this);
}

MyToolBox::~MyToolBox()
{
    delete ui;
}

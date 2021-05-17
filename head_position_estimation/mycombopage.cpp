#include "mycombopage.h"
#include "ui_mycombopage.h"

MyComboPage::MyComboPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyComboPage)
{
    ui->setupUi(this);

    ui->pushButton->setIcon(QIcon(":/img/down-arrow.png"));

    connect(ui->pushButton,&QPushButton::toggled,[=](bool checked){
        if (checked)
        {
            ui->widget->setHidden(true);
            ui->pushButton->setIcon(QIcon(":/img/left-arrow.png"));
        }
        else
        {
            ui->widget->setHidden(false);
            ui->pushButton->setIcon(QIcon(":/img/down-arrow.png"));
        }
    });
}

MyComboPage::~MyComboPage()
{
    delete ui;
}

void MyComboPage::setText(QString qs)
{
    ui->pushButton->setText(qs);
}

void MyComboPage::setWidget(QWidget* qw)
{
    ui->widget->layout()->addWidget(qw);
}

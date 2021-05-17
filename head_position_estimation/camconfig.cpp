#include "camconfig.h"
#include "ui_camconfig.h"
#include<QRadioButton>
#include<QThread>
#include<QDebug>

CamConfig::CamConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CamConfig)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        int num = selectedDevice();
        emit deliverSelected(num);
        this->close();
    });
}

CamConfig::~CamConfig()
{
    delete ui;

}

void CamConfig::setCount(int num)
{
    if (num == qlist.size())
    {
        this->show();
        return;
    }

    for (int i=0;i<qlist.size();i++)
    {
        delete qlist.at(i);
    }
    qlist.clear();

    ui->camCountLine->setText(QString::number(num));

    for (int i=0;i<num;i++)
    {
        QRadioButton* radio = new QRadioButton;
        radio->setText(QString("device ")+QString::number(i));
        radio->setStyleSheet("background:lightgray;");
        ui->widget->layout()->addWidget(radio);
        qlist.append(radio);
    }
    this->show();
//    qDebug()<<QString::number((unsigned long long)QThread::currentThreadId())<<"in widget";
}

int CamConfig::selectedDevice()
{
    for (int i=0;i<qlist.size();i++)
    {
        if (qlist.at(i)->isChecked())
            return i;
    }
    return -1;
}

#include "segwidget.h"
#include "ui_segwidget.h"

SegWidget::SegWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SegWidget)
{
    ui->setupUi(this);

    connect(ui->checkBox_Canny,&QCheckBox::clicked,[=](bool checked){
        emit CannyClicked(checked);
    });

    connect(ui->checkBox_Water,&QCheckBox::clicked,[=](bool checked){
        emit WaterClicked(checked);
    });
}

SegWidget::~SegWidget()
{
    delete ui;
}

void SegWidget::addWidget(QWidget* qw)
{
    ui->gridLayout->addWidget(qw);
}


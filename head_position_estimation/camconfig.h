#ifndef CAMCONFIG_H
#define CAMCONFIG_H

#include <QWidget>
#include<QList>
#include<QRadioButton>

namespace Ui {
class CamConfig;
}

class CamConfig : public QWidget
{
    Q_OBJECT

public:
    explicit CamConfig(QWidget *parent = nullptr);
    ~CamConfig();
    int selectedDevice();

    QList<QRadioButton*> qlist;

private:
    Ui::CamConfig *ui;

signals:
    void deliverSelected(int);
public slots:
    void setCount(int);
};

#endif // CAMCONFIG_H

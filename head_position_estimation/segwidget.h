#ifndef SEGWIDGET_H
#define SEGWIDGET_H

#include <QWidget>

namespace Ui {
class SegWidget;
}

class SegWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SegWidget(QWidget *parent = nullptr);
    ~SegWidget();

    void addWidget(QWidget*);
signals:
    void CannyClicked(bool checked);
    void WaterClicked(bool checked);

private:
    Ui::SegWidget *ui;



};

#endif // SEGWIDGET_H

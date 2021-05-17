#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QCheckBox>
#include "opencam.h"
#include"gethist.h"
#include"segwidget.h"
#include"getseg.h"
#include"dnnfacedetect.h"
#include"mytablemodel.h"
#include"camconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void openCamera();
    void showGrayLabel();
    void showHistDigram();
    void showSegment();
    void faceDetect();

private:
    Ui::MainWindow *ui;

    QImage* qimg;
    Mat src;
    QThread* myThread1; //打开摄像头线程，线程定时传出原始图像，由主线程接收
    OpenCam* openCam;
    CamConfig* camconf;

    QImage* qimg_gray;

    QThread* myThread2;
    GetHist* getHist;
    QImage* qimg_hist;
    QLabel* histLabel;

    SegWidget* segWidget;
    QThread* myThread3;
    GetSeg* getSeg;
    QImage* qimg_canny;
    QImage* qimg_water;

    QTimer* myTimer;

    bool isShowGrad = true;
    bool isShowWater = true;


    QThread* myThread4;
    QTimer* myTimer2;
    vector<Rect> faceResult;
    MyTableModel* myTableModel;
    QCheckBox* faceCheck;
    DnnFaceDetect* dfd;

public :

signals:
    void showGray();
    void showHist(QImage* qimg);
    void showSeg(QImage* qimg,vector<Rect>*);
    void getFaceDetect(QImage* qimg);

};
#endif // MAINWINDOW_H

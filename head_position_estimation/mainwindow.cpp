#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytoolbox.h"
#include<QPushButton>
#include<QDebug>
#include<QThread>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<QImage>
#include<QCheckBox>
#include"segwidget.h"
#include<QStandardItemModel>


using namespace cv;

void mat2qimg(Mat &src,QImage &qimg);
void qimg2mat(QImage &qim, Mat &dst);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openCamera();
    showGrayLabel();
    showHistDigram();
    showSegment();
    faceDetect();

    faceCheck = new QCheckBox(this);
    faceCheck->setChecked(true);
    faceCheck->setText("基于残差SSD模型的人脸检测");
    ui->widget->addItem("人脸检测",faceCheck);

    segWidget = new SegWidget;
    ui->widget->addItem("图像分割",segWidget);

    histLabel = new QLabel(this);
    histLabel->setMaximumSize(280,240);
    histLabel->setMinimumSize(280,240);
    ui->widget->addItem("直方图",histLabel);

    connect(ui->actionRefresh,&QAction::triggered,[=](){
        if (myTimer->isActive())
        {
            myTimer->stop();
        }
        emit showGray();
        emit showHist(qimg);
        emit showSeg(qimg,&faceResult);
        ui->rgbLabel->repaint();
        myTimer->start();
    });

    connect(segWidget,&SegWidget::CannyClicked,[=](bool checked){
        isShowGrad = checked;
    });
    connect(segWidget,&SegWidget::WaterClicked,[=](bool checked){
        isShowWater = checked;
    });


    myTableModel = new MyTableModel(3,4);
    QString headers[] = {"X","Y","Width","Height"};
    myTableModel->setHeader(headers,4); //设置表头
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->setModel(myTableModel);

    connect(ui->actionClose,&QAction::triggered,this,[=](){
        exit(0);
    });

//    qDebug()<<QString::number((unsigned long long)QThread::currentThreadId())<<"in main";

}

MainWindow::~MainWindow()
{
    delete ui;
    delete qimg;
    delete qimg_gray;

    delete openCam;

    delete getHist;
    delete qimg_hist;

    delete segWidget;
    delete getSeg;
    delete qimg_canny;
    delete qimg_water;

    myTimer->stop();
    myTimer2->stop();

    myThread1->quit();
    myThread2->quit();
    myThread3->quit();
    myThread4->quit();


}

void MainWindow::openCamera()
{   
    camconf = new CamConfig;

    myThread1 = new QThread;
    openCam = new OpenCam;
    openCam->moveToThread(myThread1);

    qimg = new QImage;

    connect(ui->actionCamera,&QAction::triggered,openCam,&OpenCam::getCamCount);
    connect(openCam,&OpenCam::deliverNumber,camconf,&CamConfig::setCount);
    connect(camconf,&CamConfig::deliverSelected,openCam,&OpenCam::run);

    connect(openCam,&OpenCam::deliverMat,this,[=](Mat* frame){
        src = frame->clone();
        mat2qimg(*frame,*qimg);
        ui->rgbLabel->setPixmap(QPixmap::fromImage(*qimg));

        emit showGray();
        emit showHist(qimg);

        emit showSeg(qimg,&faceResult);

    });

    myThread1->start();
}

void MainWindow::showGrayLabel()
{
    qimg_gray = new QImage;
    connect(this,&MainWindow::showGray,this,[=](){
//        *qimg_gray = qimg->convertToFormat(QImage::Format_Grayscale8,Qt::AutoDither);
        Mat temp;
        cvtColor(src,temp,cv::COLOR_BGR2GRAY);
        cvtColor(temp,temp,cv::COLOR_GRAY2BGR);
        for (int i=0;i<faceResult.size();i++)
        {
            cv::rectangle(temp,faceResult.at(i),Scalar(0,0,255));
        }
        mat2qimg(temp,*qimg_gray);
        ui->grayLabel->setPixmap(QPixmap::fromImage(*qimg_gray));
    });
}

void MainWindow::showHistDigram()
{
    myThread2 = new QThread;
    getHist = new GetHist;

    qimg_hist = new QImage;

    getHist->moveToThread(myThread2);
    connect(this,&MainWindow::showHist,getHist,&GetHist::run);
    connect(getHist,&GetHist::deliverMat,[=](Mat* hist){
        mat2qimg(*hist,*qimg_hist);
        histLabel->setPixmap(QPixmap::fromImage(*qimg_hist));
    });

    myThread2->start();
}

void MainWindow::showSegment()
{
    myThread3 = new QThread;
    getSeg = new GetSeg;

    qimg_canny = new QImage;
    qimg_water = new QImage;

    getSeg->moveToThread(myThread3);
    connect(this,&MainWindow::showSeg,getSeg,&GetSeg::run);
    connect(getSeg,&GetSeg::deliver,[=](Mat* canny,Mat* water){
        mat2qimg(*canny,*qimg_canny);
        mat2qimg(*water,*qimg_water);
        if (isShowGrad)
        {
            ui->gradLabel->setPixmap(QPixmap::fromImage(*qimg_canny));
        }
        else
        {
            ui->gradLabel->setPixmap(QPixmap());
        }

    });

    myTimer = new QTimer;
    myTimer->start(4*1000);
    connect(myTimer,&QTimer::timeout,[=](){
        if (qimg_water != NULL && isShowWater)
        {
            ui->segLabel->setPixmap(QPixmap::fromImage(*qimg_water));
//            qDebug()<<"here";
        }
        else
        {
            ui->segLabel->setPixmap(QPixmap());
        }

    });

    myThread3->start();
}

void MainWindow::faceDetect()
{
    myThread4 = new QThread;
    dfd = new DnnFaceDetect;
    dfd->moveToThread(myThread4);

    myTimer2 = new QTimer;
    myTimer2->start(500);

    connect(myTimer2,&QTimer::timeout,this,[=](){
        if (qimg != NULL && faceCheck->isChecked())
        {
            faceResult.clear();
            myTableModel->clearItemT();
            emit getFaceDetect(qimg);
        }

    });

    connect(this,&MainWindow::getFaceDetect,dfd,&DnnFaceDetect::run);
    connect(dfd,&DnnFaceDetect::deliverMat,[=](vector<Rect> rects){
        faceResult = rects;

        for (int i=0;i<rects.size();i++)
        {
            myTableModel->setItemT(i,0,QString::number(rects[i].x));
            myTableModel->setItemT(i,1,QString::number(rects[i].y));
            myTableModel->setItemT(i,2,QString::number(rects[i].width));
            myTableModel->setItemT(i,3,QString::number(rects[i].height));
        }

    });

    myThread4->start();
}

void mat2qimg(Mat &src,QImage &qimg)
{
    if (src.channels()==3)
    {
        cvtColor(src,src,CV_BGR2RGB);
        qimg = QImage((const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_RGB888 );
    }
    else
    {
        qimg = QImage((const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_Grayscale8 );
    }
    return;
}


#ifndef OPENCAM_H
#define OPENCAM_H

#include <QObject>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<QTimer>
#include"camconfig.h"

using namespace cv;

class OpenCam : public QObject
{
    Q_OBJECT
public:
    OpenCam();
    ~OpenCam();

    VideoCapture cap;
    Mat frame;
    QImage qimg;
    int selectDev = -1;
    int total_device;
    int max = 5;

private:
    QTimer* timerCap = NULL;



signals:
    void deliverMat(Mat*);
    void deliverNumber(int);
public slots:
    void run(int selected);
    void getCamCount();




};

#endif // OPENCAM_H

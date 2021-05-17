#include "opencam.h"
#include<QThread>
#include<QDebug>


void mat2qimg(Mat &src,QImage &qimg);


OpenCam::OpenCam()
{

}

OpenCam::~OpenCam()
{
    delete timerCap;
}

void OpenCam::run(int selected)
{
    if (selected == -1)
    {
        if (cap.isOpened())
        {
            cap.release();
        }
        timerCap->stop();
        selectDev = selected;
        return;
    }

    if (selected == selectDev)
        return;
    if (timerCap == NULL)
    {
        timerCap = new QTimer;
        //使用定时器定时发送消息
        connect(timerCap,&QTimer::timeout,this,[=](){
            if (cap.isOpened())
            {
                cap >> frame;
                if (!frame.empty())
                {
                    cv::resize(frame,frame,Size(320,240));
                    emit deliverMat(&frame);
    //                qDebug()<<"发送消息";
                }
            }
        });
    }

    selectDev = selected;
    if (cap.isOpened())
    {
        timerCap->stop();
        cap.release();
    }

    cap.open(selectDev);
//                cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
                //设置采集分辨率
//                cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
//                cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);

    double rate = cap.get(cv::CAP_PROP_FPS);
    timerCap->start(1000/rate);



}

void OpenCam::getCamCount()
{
    if (cap.isOpened())
    {
        emit deliverNumber(total_device);
        return;
    }

    max = 5;
    int i=max-1;
    for (;i>=0;i--)
    {
        if (cap.open(i))
        {
            cap.release();
            break;
        }
    }
    total_device = i+1;
    qDebug()<<total_device;
    emit deliverNumber(total_device);
//    qDebug()<<QString::number((unsigned long long)QThread::currentThreadId())<<" in openCam";
}


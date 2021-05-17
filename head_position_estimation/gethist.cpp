#include "gethist.h"
#include<QImage>
#include<vector>
#include<QDebug>

void qimg2mat(QImage &qim, Mat &dst);

GetHist::GetHist(QObject *parent) : QObject(parent)
{

}

void GetHist::run(QImage* qimg)
{
    if (qimg == NULL)
        return;

    qimg2mat(*qimg,frame);
    //-------------【2】图像通道的分离，3个通道B、G、R------------------------
    std::vector<Mat> rgb_channel;
    split(frame, rgb_channel);

    //--------------【3】初始化参数
    int bins = 256;
    int histsize[] = { bins };
    float range[] = { 0, 256 };
    const float* histRange = { range };
    Mat  b_Hist, g_Hist, r_Hist;
    //-------------【4】计算各个通道的直方图--------------------------------------
    calcHist(&rgb_channel[0], 1, 0, Mat(), b_Hist, 1, histsize, &histRange, true, false); //B-通道
    calcHist(&rgb_channel[1], 1, 0, Mat(), g_Hist, 1, histsize, &histRange, true, false); //G-通道
    calcHist(&rgb_channel[2], 1, 0, Mat(), r_Hist, 1, histsize, &histRange, true, false); //R-通道
    //-------------【5】设置直方图绘图参数----------------------------------------------------
    int hist_h = 360;
    int hist_w = bins * 3;
    int bin_w = cvRound((double)hist_w / bins);
    histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(186, 189, 182));//创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像
    //-------------【6】将直方图归一化到[0,histImage.rows]  ------------------------------------------------------------------------------
    normalize(b_Hist, b_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //B-通道
    normalize(g_Hist, g_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //G-通道
    normalize(r_Hist, r_Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());  //R-通道
    //--------------【7】绘制直方图  ----------------------------------------------------------------
    for (int i = 1; i < bins; i++)
    {
        //绘制B通道的直方图信息
        line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_Hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(b_Hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
        //绘制G通道的直方图信息
        line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_Hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(g_Hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
        //绘制R通道的直方图信息
        line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_Hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(r_Hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
    }
    cv::resize(histImage,histImage,Size(280,240));

    emit deliverMat(&histImage);
}

void qimg2mat(QImage &qim, Mat &dst)
{
    switch (qim.format())
    {
    case QImage::Format_RGB888:
        dst = Mat(qim.height(), qim.width(),
            CV_8UC3,(void*)qim.constBits(),qim.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        dst = Mat(qim.height(), qim.width(),
            CV_8UC1,(void*)qim.constBits(),qim.bytesPerLine());
        break;
    }
    return;
}

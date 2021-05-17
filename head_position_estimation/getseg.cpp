#include "getseg.h"
#include<QImage>

#include<time.h>
#include<QDebug>


void qimg2mat(QImage &qim, Mat &dst);
void fillHole(const cv::Mat srcimage, cv::Mat &dstimage);

Vec3b RandomColor(int value)    //生成随机颜色函数
{
    value = value % 255;  //生成0~255的随机数
    RNG rng;
    int aa = rng.uniform(0, value);
    int bb = rng.uniform(0, value);
    int cc = rng.uniform(0, value);
    return Vec3b(aa, bb, cc);
}


GetSeg::GetSeg(QObject *parent) : QObject(parent)
{
    marks = Mat(240,320, CV_32S,Scalar::all(0));   //Opencv分水岭第二个矩阵参数
    PerspectiveImage = new Mat(240,320, CV_8UC3,Scalar::all(0));

    for (size_t i=0; i<500; i++)
    {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)r, (uchar)g, (uchar)b));
    }
}

GetSeg::~GetSeg()
{
//    delete marks;
    delete PerspectiveImage;
}


void GetSeg::run(QImage* qimg,vector<Rect>* faceR)
{
    if (qimg == NULL)
        return;
    marks.setTo(0);

    qimg2mat(*qimg,rgb_image);
    GaussianBlur(rgb_image, rgb_image_blur, Size(5, 5), 0, 0);
    Canny(rgb_image_blur, rgb_image_canny, 20, 40, 3, false);

#if 1
//    Mat temp1 = rgb_image_canny.clone();

    Mat temp = Mat::zeros(240,320,CV_8UC1);
//    fillHole(temp1,temp);

    for (int i=0;i<faceR->size();i++)
    {
        addMarker(temp,faceR->at(i));
    }
//    imshow("add markers",temp);

    cv::findContours(temp, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point());

    for (int index=0; index<contours.size(); index++)
    {
        //对marks进行标记，对不同区域的轮廓进行编号，相当于设置注水点，有多少轮廓，就有多少注水点
        drawContours(marks, contours, index, Scalar::all(index + 1), 1, 8, hierarchy);
    }
    if (rgb_image.size != marks.size)
        return;
    cv::watershed(rgb_image, marks);


//    对每一个区域进行颜色填充
    int rows = marks.rows;
    int cols = marks.cols;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = marks.at<int>(i, j);
            if (index>0 && index <= static_cast<int>(contours.size()))
            {
                PerspectiveImage->at<Vec3b>(i,j) = colors[index-1];
            }
            else
            {
                PerspectiveImage->at<Vec3b>(i,j) = Vec3b(0,0,0);
            }
        }
    }

#endif
    emit deliver(&rgb_image_canny,PerspectiveImage);
}

void GetSeg::addMarker(Mat& img,Rect rect)
{
    if (img.channels() != 1)
        return;
    for (int i=0;i<img.rows;i++)
    {
        uchar* data = img.ptr<uchar>(i);
        for (int j=0;j<img.cols;j++)
        {
            if (rect.contains(Point(j,i)))
            {
                data[j] = 255;
            }
        }
    }
    Mat temp = img.clone();
    cv::dilate(img,img,cv::getStructuringElement(cv::MORPH_ELLIPSE,Size(3,3)),Point(-1,-1),40);
    img = ~img;
    cv::erode(temp,temp,cv::getStructuringElement(cv::MORPH_ELLIPSE,Size(3,3)),Point(-1,-1),10);
    img = img + temp;
}


void fillHole(const cv::Mat srcimage, cv::Mat &dstimage)
{
    Size m_Size = srcimage.size();
    Mat temimage = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//延展图像
    srcimage.copyTo(temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
    floodFill(temimage, Point(0, 0), Scalar(255));
    Mat cutImg;//裁剪延展的图像
    temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
    dstimage = srcimage | (~cutImg);
}

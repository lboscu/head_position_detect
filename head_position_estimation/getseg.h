#ifndef GETSEG_H
#define GETSEG_H

#include <QObject>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

using namespace cv;
using namespace std;

class GetSeg : public QObject
{
    Q_OBJECT
public:
    explicit GetSeg(QObject *parent = nullptr);
    ~GetSeg();


private:
    vector<Vec3b> colors;

    cv::Mat rgb_image;
    cv::Mat rgb_image_blur;
    cv::Mat rgb_image_canny;
    vector<vector<Point>>contours;
    vector<Vec4i>hierarchy;
    Mat marks;
    Mat* PerspectiveImage;

    void addMarker(Mat& img,Rect rect);


signals:
    void deliver(Mat*,Mat*);
public slots:
    void run(QImage*,vector<Rect>*);

};

#endif // GETSEG_H

#ifndef DNNFACEDETECT_H
#define DNNFACEDETECT_H

#include<QObject>
#include<QImage>
#include<opencv2/opencv.hpp>
#include<opencv2/dnn/dnn.hpp>
#include<string>

using namespace std;
using namespace cv;

class DnnFaceDetect:public QObject
{
    Q_OBJECT
private:
    string _modelbinary, _modeldesc;
    dnn::Net _net;
public:
    //构造函数 传入模型文件
    DnnFaceDetect();
//    explicit DnnFaceDetect(QObject *parent = nullptr);
    DnnFaceDetect(string modelBinary, string modelDesc);

    ~DnnFaceDetect();
    //置信阈值
    float confidenceThreshold;
    double inScaleFactor;
    size_t inWidth;
    size_t inHeight;
    Scalar meanVal;

    //检测结果
    vector<Rect>* dsts;

    //初始化DNN网络
    bool initdnnNet();

    //人脸检测
    void detect(Mat* frame);

signals:
    void deliverMat(vector<Rect>);

public slots:
    void run(QImage*);

private:
    Mat src;
    Mat inputBlob;
    Mat detection;
};

#endif // DNNFACEDETECT_H

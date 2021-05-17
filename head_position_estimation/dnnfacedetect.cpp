#include "dnnfacedetect.h"
#include<QDebug>
#include<QFileInfo>

void qimg2mat(QImage &qim, Mat &dst);

DnnFaceDetect::DnnFaceDetect()
{
    //定义模型文件
    string modelBinary = "/home/lb/opencv/demo/head_position_estimation/opencv_face_detector_uint8.pb";
    string modelDesc = "/home/lb/opencv/demo/head_possition_estimation/opencv_face_detector.pbtxt";
    assert(QFileInfo(QString(modelBinary.c_str())).exists());
    assert(QFileInfo(QString(modelDesc.c_str())).exists());


    //初始化
    _modelbinary = modelBinary;
    _modeldesc = modelDesc;

    //初始化置信阈值
    confidenceThreshold = 0.6;
    inScaleFactor = 0.5;
    inWidth = 300;
    inHeight = 300;
    meanVal = Scalar(104.0, 177.0, 123.0);

    //检测结果
    dsts = new vector<Rect>();

    if (!initdnnNet())
    {
        qDebug() << "初始化DNN人脸检测失败！";
    }
}


//构造函数
DnnFaceDetect::DnnFaceDetect(string modelBinary, string modelDesc)
{
    _modelbinary = modelBinary;
    _modeldesc = modelDesc;

    //初始化置信阈值
    confidenceThreshold = 0.6;
    inScaleFactor = 0.5;
    inWidth = 300;
    inHeight = 300;
    meanVal = Scalar(104.0, 177.0, 123.0);
}

DnnFaceDetect::~DnnFaceDetect()
{
    _net.~Net();
    delete dsts;
}

//初始化dnnnet
bool DnnFaceDetect::initdnnNet()
{
    _net = dnn::readNetFromTensorflow(_modelbinary, _modeldesc);
    _net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    _net.setPreferableTarget(dnn::DNN_TARGET_CPU);

    return !_net.empty();
}

//人脸检测
void DnnFaceDetect::detect(Mat* frame)
{

    dsts->clear();
    // 修改通道数
    if (frame->channels() == 4)
        cvtColor(*frame, *frame, COLOR_BGRA2BGR);

    // 输入数据调整
    inputBlob = dnn::blobFromImage(*frame, inScaleFactor,
    Size(inWidth, inHeight), meanVal, false, false);
    _net.setInput(inputBlob, "data");

    //人脸检测
    detection = _net.forward("detection_out");

    Mat detectionMat(detection.size[2], detection.size[3],
    CV_32F, detection.ptr<float>());

    //检测出的结果进行绘制和存放到dsts中
    for (int i = 0; i < detectionMat.rows; i++)
    {
        //置值度获取
        float confidence = detectionMat.at<float>(i, 2);
        //如果大于阈值说明检测到人脸
        if (confidence > confidenceThreshold)
        {
            //计算矩形
            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame->cols);
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame->rows);
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame->cols);
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame->rows);
            //生成矩形
            Rect rect((int)xLeftBottom, (int)yLeftBottom,
            (int)(xRightTop - xLeftBottom),
            (int)(yRightTop - yLeftBottom));

            //矩形存放到dsts数组中
            dsts->push_back(rect);
        }
    }

  return;
}

void DnnFaceDetect::run(QImage* qimg)
{
    if (qimg == NULL)
        return;
    qimg2mat(*qimg,src);

    try
    {
        if (!src.empty())
        {
            this->detect(&src);
            if (!dsts->empty())
            {
                emit deliverMat(*dsts);
            }
        }
    }
    catch (const std::exception & ex)
    {
        qDebug() << ex.what();
    }

}

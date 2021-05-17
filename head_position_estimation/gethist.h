#ifndef GETHIST_H
#define GETHIST_H

#include <QObject>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

using namespace cv;

class GetHist : public QObject
{
    Q_OBJECT
public:
    explicit GetHist(QObject *parent = nullptr);
    Mat histImage;

signals:
    void deliverMat(Mat*);

public slots:
    void run(QImage*);

private:
    Mat frame;
};

#endif // GETHIST_H

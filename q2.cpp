#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <string.h>
#include "basic.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;

void runQ2(){

    int size = 201;
    int width = 50;
    int center = 100;
    Mat boxCircle = Mat::zeros(Size(size, size), CV_8U);
    for (int i = 0; i < boxCircle.cols; i++){
        for (int j = 0; j < boxCircle.rows; j++){

            if (pow((i - center), 2) + pow((j - center), 2) < pow((width), 2))
            boxCircle.at<uchar>(j, i) = 255;
        }
    }
    namedWindow("boxCircle", 1);
    imshow("boxCircle", boxCircle);
}
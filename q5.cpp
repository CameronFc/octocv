#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <string.h>
#include "basic.h"
#define _USE_MATH_DEFINES
#include <math.h>

void runQ5(){
    Mat image = imread("images/midterm3.bmp");
    namedWindow("Original Image", 1);
    imshow("Original Image", image);

    //Identity matrix e; used to produce gaussian high-pass kernel
    Mat id = Mat::zeros(Size(25, 25), CV_8U);
    id.at<uchar>(13, 13) = 1;
    id.convertTo(id, CV_32F);
    //Mat ident;
    //filter2D(image, ident, -1, id);
    //namedWindow("Ident Image", 1);
    //imshow("Ident Image", ident);

    //Generate a 2D gaussian kernel
    int ksize = 25;
    cv::Mat kernel(ksize, ksize, CV_64F);
    int rows = kernel.rows;
    int cols = kernel.cols;
    double halfSize = (double)ksize / 2.0;
    double sigma = 10;
    for (size_t i = 0; i<rows; i++)
    for (size_t j = 0; j<cols; j++)
    {
        double x = (double)j - halfSize;
        double y = (double)i - halfSize;
        kernel.at<double>(j, i) = (1.0 / (2*M_PI*pow(sigma, 2))) * pow(2.718281828, -(x*x + y*y) / (2 * sigma*sigma));
    }
    //namedWindow("kernel Image", 1);
    //imshow("kernel Image", kernel);

    Mat lowPassed;
    filter2D(image, lowPassed, -1, kernel);
    namedWindow("Lowpassed Image", 1);
    imshow("Lowpassed Image", lowPassed);

    Mat highPass;
    kernel.convertTo(kernel, CV_32F);
    id.convertTo(id, CV_32F);
    subtract(id, kernel, highPass);
    Mat highPassed;
    filter2D(image, highPassed, -1, highPass);
    namedWindow("Highpassed Image", 1);
    imshow("Highpassed Image", highPassed);

    Mat hl;
    filter2D(highPassed, hl, -1, kernel);
    namedWindow("High->LowPassed", 1);
    imshow("High->LowPassed", hl);
    imwrite("images/highLow.bmp", hl);

    Mat lh;
    filter2D(lowPassed, lh, -1, highPass);
    namedWindow("Low->HighPassed", 1);
    imshow("Low->HighPassed", lh);
    imwrite("images/lowHigh.bmp", lh);
}
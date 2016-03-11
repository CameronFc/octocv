#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <string.h>
#include "basic.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;

void runQ1(){
    Mat whiteBlack = Mat::zeros(Size(32, 32), CV_8U);
    for (int i = 0; i < whiteBlack.cols/2; i++){
        for (int j = 0; j < whiteBlack.rows; j++){
            whiteBlack.at<uchar>(j, i) = 255;
        }
    }
    //namedWindow("whiteBlack", 1);
    //imshow("whiteBlack", whiteBlack);

    Mat checkered = Mat::zeros(Size(32, 32), CV_8U);
    for (int i = 0; i < whiteBlack.cols; i++){
        for (int j = 0; j < whiteBlack.rows; j++){
            int xquad = i / 8;
            int yquad = j / 8;

            if ((xquad + yquad) % 2 == 0){
                checkered.at<uchar>(j, i) = 255;
            }
        }
    }
    //namedWindow("checkered", 1);
    //imshow("checkered", checkered);

    Mat averagedBW;
    boxFilter(whiteBlack, averagedBW, -1, Size(3,3));
    //namedWindow("averagedBW", 1);
    //imshow("averagedBW", averagedBW);

    Mat averagedCheckered;
    boxFilter(checkered, averagedCheckered, -1, Size(3, 3));
    //namedWindow("averagedCheckered", 1);
    //imshow("averagedCheckered", averagedCheckered);

    Mat images[] = { whiteBlack, averagedBW, checkered, averagedCheckered };

    std::cout << "Advance to next image by pressing any key..." << std::endl;

    for (int i = 0; i < 4; i++){

        Mat image = images[i];

        //Calculate the histogram
        Mat hist;
        int histSize = 32;
        float hranges[] = { 0, 256 };
        const float* ranges[] = { hranges };
        cv::calcHist(&image, 1, { 0 }, Mat(), hist, 1, &histSize, ranges);

        // Plot the histogram
        // From: http://opencvexamples.blogspot.com/2013/10/histogram-calculation.html
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound((double)hist_w / histSize);
        Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
        for (int i = 1; i < histSize; i++)
        {
            line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
                Scalar(255, 0, 0), 2, 8, 0);
        }
        namedWindow("Original Image", 1);    imshow("Original Image", image);
        namedWindow("Histogram", 1);    imshow("Histogram", histImage);
        waitKey(0);
    }

}
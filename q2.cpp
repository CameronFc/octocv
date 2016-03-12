#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <string.h>
#include "basic.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;

void getMag(Mat src, Mat& dest){
    // From: docs.opencv.org/3.1.0/d8/d01/tutorial_discrete_fourier_transform.html
    Mat I = src.clone();
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize(I.rows);
    int n = getOptimalDFTSize(I.cols); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    dest = magI.clone();
}

void rearrangeSpectrum(Mat src, Mat& dest){
    Mat magI = src.clone();
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    dest = magI.clone();
}

void rearrange1D(Mat src, Mat& dest){
    Mat magI = src.clone();
    int center = magI.cols / 2;
    Mat q0(magI, Rect(0, 0, center, 1));
    Mat q1(magI, Rect(center, 0, center, 1));

    Mat tmp;
    q0.copyTo(tmp);
    q1.copyTo(q0);
    tmp.copyTo(q1);
    dest = magI.clone();
}

void drawHist(Mat src, Mat& dest){
    Mat magI = src.clone();
    Mat hist = magI.clone();
    int size = magI.cols;
    int histSize = size;

    // Plot the histogram
    // From: http://opencvexamples.blogspot.com/2013/10/histogram-calculation.html
    int hist_w = size * 2; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++){
        line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
        Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
        Scalar(255, 0, 0), 2, 8, 0);
    }
    namedWindow("Histogram", 1);
    imshow("Histogram", histImage);
}

void runQ2(){

    // 2D boxfilter used in part e
    int size = 400;
    int center = (size - 1) / 2;
    int width = center / 2;
    Mat boxCircle = Mat::zeros(Size(size, size), CV_8U);
    for (int i = 0; i < boxCircle.cols; i++){
        for (int j = 0; j < boxCircle.rows; j++){

            if (pow((i - center), 2) + pow((j - center), 2) < pow((width), 2))
            boxCircle.at<uchar>(j, i) = 255;
        }
    }

    // 1D boxfilter used in parts a-d
    Mat boxLine = Mat::zeros(Size(size, 1), CV_8U);
    for (int i = 0; i < boxLine.cols; i++){
        if ( pow((i - center), 2) < pow(width, 2) )
            boxLine.at<uchar>(0, i) = 255;
    }
 
    // Question H
    /*
    string images[] = { "Q2H3.bmp", "Q2H4.bmp", "Q2H5.bmp" };
    for (int i = 0; i < 3; i++){
        Mat image = imread("images/" + images[i]);
        cvtColor(image, image, CV_RGB2GRAY);
        image.convertTo(image, CV_8U);

        Mat magI;
        getMag(boxCircle, magI);
        rearrangeSpectrum(magI, magI);
        
        normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                                  // viewable image form (float between values 0 and 1).
        // Scale values for writing
        magI = magI * 255.0;
        imwrite("images/mag_" + images[i], magI);

    }
    */
    
    // Question A
    /*
    Mat magI;
    getMag(boxLine, magI);
    rearrange1D(magI, magI);
    normalize(magI, magI, 0, 1, NORM_MINMAX);
    Mat hist;
    drawHist(magI, hist);
    */

    // Question E
    /*
    Mat magI;
    getMag(boxCircle, magI);
    rearrangeSpectrum(magI, magI);
    normalize(magI, magI, 0, 1, NORM_MINMAX);
    namedWindow("input", 1);
    imshow("input", boxCircle);
    namedWindow("result", 1);
    imshow("result", magI);
    */

}
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <string.h>
#include "basic.h"

using namespace cv;

void runBasic(int argc, char** argv){
    /// Initialize values
    std::cout << " Basic Linear Transforms " << std::endl;
    std::cout << "-------------------------" << std::endl;
    //std::cout << "* Enter the alpha value [1.0-3.0]: "; std::cin >> alpha;
    //std::cout << "* Enter the beta value [0-100]: "; std::cin >> beta;

    /// Do the operation new_image(i,j) = alpha*image(i,j) + beta

    //for (int y = 0; y < image.rows; y++)
    //{
    //    for (int x = 0; x < image.cols; x++)
    //    {
    //        for (int c = 0; c < 3; c++)
    //        {
    //            new_image.at<Vec3b>(y, x)[c] =
    //                saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
    //        }
    //    }
    //}

    //int rfs = 7;
    //int norm = (2 * rfs + 1) * (2 * rfs + 1);
    //for (int y = 0; y < image.rows; y++)
    //{
    //    for (int x = 0; x < image.cols; x++)
    //    {
    //        double newColorIntensity = 0.0;
    //        for (int c = 0; c < 3; c++){
    //            // Average over receptive field
    //            for (int i = -rfs; i <= rfs; i++){
    //                for (int j = -rfs; j <= rfs; j++){
    //                    // Account for zero padding
    //                    if (y + j < 0 || y + j >= image.rows || x + i < 0 || x + i >= image.cols){
    //                        newColorIntensity += 0.0;
    //                    }
    //                    else{
    //                        newColorIntensity += (int)(image.at<Vec3b>(y + j, x + i)[c]);
    //                        //std::cout << "New intensity after adding: " << newColorIntensity << std::endl;
    //                    }
    //                }
    //            }
    //        newColorIntensity = newColorIntensity / norm;
    //        //std::cout << "My old intensity: " << (int)image.at<Vec3b>(y, x)[c] << " My new intensity: " << newColorIntensity << std::endl;
    //        new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(newColorIntensity);
    //        }
    //    }
    //}

    /// Create Windows
    //namedWindow("Original Image", 1);
    //namedWindow("New Image", 1);

    /// Show stuff
    //imshow("Original Image", image);
    //imshow("New Image", new_image);

    /// Read image given by user
    for (int i = 1; i < argc; i++){
        size_t lastindex = std::string(argv[i]).find_last_of(".");
        string rawname = std::string(argv[i]).substr(0, lastindex);
        string dest = "J:\\Visual Studio 2013\\Projects\\OpencvMaster\\OpencvMaster\\images\\";
        string outputName = dest + rawname;

        Mat image = imread(argv[i]);
        Mat new_image = Mat::zeros(image.size(), image.type());
        boxFilter(image, new_image, -1, cv::Size(15, 15), Point(-1, -1), true, BORDER_CONSTANT);
        imwrite(outputName + "_boxed.bmp", new_image);

        new_image = Mat::zeros(image.size(), image.type());
        blur(image, new_image, Size(7, 7));
        imwrite(outputName + "_blurred.bmp", new_image);

        new_image = Mat::zeros(image.size(), image.type());
        int dilation_type = MORPH_RECT;
        int dilation_size = 2;
        Mat element = getStructuringElement(dilation_type,
            Size(2 * dilation_size + 1, 2 * dilation_size + 1),
            Point(dilation_size, dilation_size));
        dilate(image, new_image, element);
        imwrite(outputName + "_dilated.bmp", new_image);

        element = getStructuringElement(MORPH_ERODE,
            Size(2 * dilation_size + 1, 2 * dilation_size + 1),
            Point(dilation_size, dilation_size));
        new_image = Mat::zeros(image.size(), image.type());
        erode(image, new_image, element);
        imwrite(outputName + "_eroded.bmp", new_image);

        new_image = Mat::zeros(image.size(), image.type());
        cv::cvtColor(image, new_image, CV_BGR2GRAY);
        cornerHarris(new_image, new_image, 3, 13, 0.10);
        imwrite(outputName + "_harris.bmp", new_image);

        new_image = Mat::zeros(image.size(), image.type());
        Sobel(image, new_image, 3, 1, 1);
        imwrite(outputName + "_sobel.bmp", new_image);

        new_image = Mat::zeros(image.size(), image.type());
        Laplacian(image, new_image, 3);
        imwrite(outputName + "_laplacian.bmp", new_image);

        Mat dup = Mat::zeros(image.size(), image.type());
        image.copyTo(dup);
        new_image.convertTo(new_image, CV_8UC3);
        std::cout << dup.type() << " " << new_image.type() << std::endl;
        subtract(dup, new_image, new_image);
        imwrite(outputName + "_enhanced.bmp", new_image);
    }

}
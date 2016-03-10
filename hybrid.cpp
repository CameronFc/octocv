#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include "basic.h"

using namespace cv;

void runHybrid(int argc, char** argv){
    for (int i = 1; i < argc; i++){
        size_t lastindex = std::string(argv[i]).find_last_of(".");
        string rawname = std::string(argv[i]).substr(0, lastindex);
        string imdir = "C:\\Users\\Cameron\\Documents\\Visual Studio 2013\\Projects\\OpencvMaster\\OpencvMaster\\images\\";
        string outputName = imdir + rawname;
        std::cout << outputName << endl;



        /*std::cout << gMat.type();
        for (int j = 0; j < gMat.rows; j++){
            for (int k = 0; k < gMat.rows; k++){
                gMat(j, k) = 1.0;
            }
        }*/

        //Mat ones = Mat::ones(gMat.size(), gMat.type());

        //std::cout << ones << std::endl;

        //gMat = -gMat + ones;
        //std::cout << (sum(ones)[0]) << endl;
        //cv::divide(gMat, ones * sum(ones)[0], gMat);

        //normalize(gMat, gMat);
        Mat image = imread(imdir + argv[i]);

        Mat_<float> gMat = getGaussianKernel(51, -1, CV_64F);
        Mat gImage = Mat::zeros(gMat.size(), gMat.type());
        imwrite(imdir + "gFilter.bmp", gImage);
        gMat.convertTo(gMat, CV_32FC1);

        
        dft(gMat, gMat, cv::DFT_SCALE);
        std::cout << gMat << std::endl;
        //Mat ones = Mat::ones(gMat.size(), gMat.type());
        //add(-gMat, ones, gMat);
        dft(gMat, gMat, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

        Mat new_image = Mat::zeros(image.size(), image.type());
        sepFilter2D(image, new_image, -1, gMat, gMat);
        //boxFilter(image, new_image, -1, cv::Size(15, 15), Point(-1, -1), true, BORDER_CONSTANT);
        imwrite(outputName + "_boxed.bmp", new_image);
        //imwrite(outputName + "_dft.bmp", finalImage);

        //cv::cvtColor(image, fImage, cv::COLOR_BGR2GRAY);
        //imwrite(outputName + "_greyed.bmp", fImage);

        Mat channels[3];
        Mat fImage;
        Mat finalImage;
        split(image, channels);
        std::cout << "Hi, I didn't get here.";
        for (int j = 0; j < 3; j++){

            // Change channel into floating point type expected by fourier transform.
            channels[j].convertTo(channels[j], CV_32FC1);

            // FFT
            std::cout << "Direct transform...\n";
            Mat fourierTransform;
            dft(channels[j], fourierTransform, DFT_SCALE);

            //Perform processing in frequency domain
            Mat gMat = getGaussianKernel(channels[j].cols, -1);
            gMat.convertTo(gMat, CV_32FC1);
            dft(gMat, gMat, DFT_SCALE);
            std::cout << "channel type: " << channels[j].type() << " filter type: " << gMat.type() << std::endl;
            std::cout << "channel size: " << channels[j].size() << " filter size: " << gMat.size() << std::endl;


            //gMat = 1.0 - gMat;
            //std::cout << gMat << std::endl;
            //for (int k = 0; k < channels[j].rows; k++){
            //    Mat row;
            //    transpose(channels[j].row(k), row);
            //    //std::cout << "channel type: " << row.type() << " filter type: " << gMat.type() << std::endl;
            //    //std::cout << "channel size: " << row.size() << " filter size: " << gMat.size() << std::endl;
            //    mulSpectrums(row, gMat, row, 0);

            //    transpose(row, row);
            //    channels[j].row(k) = row + 0.0;
            //}

            // IFFT
            std::cout << "Inverse transform...\n";
            cv::Mat inverseTransform;
            cv::dft(fourierTransform, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

        }

        merge(channels, 3, finalImage);
        imwrite(outputName + "_final.bmp", finalImage);


        //std::cout << gMat << std::endl;
    }
}
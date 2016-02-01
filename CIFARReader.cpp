// readCIFAR10.cc
// 
// feel free to use this code for ANY purpose
// author : Eric Yuan 
// my blog: http://eric-yuan.me/

// Modified from original

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <iostream>

#define ATD at<double>
#define elif else if

using namespace cv;
using namespace std;

void
read_batch(string filename, vector<Mat> &vec, Mat &label){
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        int number_of_images = 10000;
        int n_rows = 32;
        int n_cols = 32;
        for (int i = 0; i < number_of_images; ++i)
        {
            unsigned char tplabel = 0;
            file.read((char*)&tplabel, sizeof(tplabel));
            vector<Mat> channels;
            Mat fin_img = Mat::zeros(n_rows, n_cols, CV_8UC3);
            for (int ch = 0; ch < 3; ++ch){
                Mat tp = Mat::zeros(n_rows, n_cols, CV_8UC1);
                for (int r = 0; r < n_rows; ++r){
                    for (int c = 0; c < n_cols; ++c){
                        unsigned char temp = 0;
                        file.read((char*)&temp, sizeof(temp));
                        tp.at<uchar>(r, c) = (int)temp;
                    }
                }
                channels.push_back(tp);
            }
            merge(channels, fin_img);
            vec.push_back(fin_img);
            label.ATD(0, i) = (double)tplabel;
        }
    }
}

Mat
concatenateMat(vector<Mat> &vec){

    int height = vec[0].rows;
    int width = vec[0].cols;
    Mat res = Mat::zeros(height * width, vec.size(), CV_64FC1);
    for (int i = 0; i<vec.size(); i++){
        Mat img(height, width, CV_64FC1);
        Mat gray(height, width, CV_8UC1);
        cvtColor(vec[i], gray, CV_RGB2GRAY);
        gray.convertTo(img, CV_64FC1);
        // reshape(int cn, int rows=0), cn is num of channels.
        Mat ptmat = img.reshape(0, height * width);
        Rect roi = cv::Rect(i, 0, ptmat.cols, ptmat.rows);
        Mat subView = res(roi);
        ptmat.copyTo(subView);
    }
    divide(res, 255.0, res);
    return res;
}

Mat
concatenateMatC(vector<Mat> &vec){

    int height = vec[0].rows;
    int width = vec[0].cols;
    Mat res = Mat::zeros(height * width * 3, vec.size(), CV_64FC1);
    for (int i = 0; i<vec.size(); i++){
        Mat img(height, width, CV_64FC3);
        vec[i].convertTo(img, CV_64FC3);
        vector<Mat> chs;
        split(img, chs);
        for (int j = 0; j < 3; j++){
            Mat ptmat = chs[j].reshape(0, height * width);
            Rect roi = cv::Rect(i, j * ptmat.rows, ptmat.cols, ptmat.rows);
            Mat subView = res(roi);
            ptmat.copyTo(subView);
        }
    }
    divide(res, 255.0, res);
    return res;
}

void
read_CIFAR10(Mat &trainX, Mat &testX, Mat &trainY, Mat &testY){

    int NUM_CIFAR_BATCHES = 5;
    string path = "C:/Users/s1373037/Downloads/";
    string filename;

    std::cout << "Starting to read batches... \n";
    for (int i = 1; i <= NUM_CIFAR_BATCHES + 1; i++){

        // For all the batches that are not the test case, set path to batch
        if (i != 6){
            filename = path + "cifar-10-batches-bin/data_batch_" + to_string(i) + ".bin";
        }
        else{
            filename = path + "cifar-10-batches-bin/test_batch.bin";
        }

        // Read the batch, then concatenate labels with data
        vector<Mat> batch;
        Mat label = Mat::zeros(1, 10000, CV_64FC1);
        read_batch(filename, batch, label);
        std::cout << "Read " << to_string(i) << " batch\n";
        Mat mt = concatenateMat(batch);

        // For all the batches that are not the test case, place in appropriate spot in training section
        if (i != 6){
            Rect roi = cv::Rect(mt.cols * (i-1), 0, mt.cols, trainX.rows);
            Mat subView = trainX(roi);
            mt.copyTo(subView);
            roi = cv::Rect(label.cols * (i-1), 0, label.cols, 1);
            subView = trainY(roi);
            label.copyTo(subView);
        }
        else{
            mt.copyTo(testX);
            label.copyTo(testY);
        }
    }
}

int
runReader()
{

    Mat trainX, testX;
    Mat trainY, testY;
    trainX = Mat::zeros(1024, 50000, CV_64FC1);
    testX = Mat::zeros(1024, 10000, CV_64FC1);
    trainY = Mat::zeros(1, 50000, CV_64FC1);
    testX = Mat::zeros(1, 10000, CV_64FC1);

    read_CIFAR10(trainX, testX, trainY, testY);

    return 0;
}
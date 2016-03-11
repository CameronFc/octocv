#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

void runBasic(int, char**);
void read_batch(string filename, vector<Mat> &vec, Mat &label);
void runHybrid(int, char**);
Mat concatenateMat(vector<Mat> &vec);
Mat concatenateMatC(vector<Mat> &vec);
void read_CIFAR10(Mat &trainX, Mat &testX, Mat &trainY, Mat &testY);
int runReader();
void runQ5();
void runQ1();
void runQ2();

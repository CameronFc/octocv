#include <opencv/highgui.h>
#include "basic.h"

using namespace cv;

int main(int argc, char** argv)
{

    // runBasic(argc, argv);
    runReader();

    /// Wait until user press some key
    waitKey();
    return 0;
}
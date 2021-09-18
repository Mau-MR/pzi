
#include <opencv2/highgui.hpp>
#include "Hardware.h"
#include<iostream>
#include "opencv2/videoio.hpp"

using namespace cv;
using namespace std;

void Hardware::openCamara() {
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(0)) {
        cout << "Camara not found" << endl;
        return;
    }

    for (;;) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break; // end of video stream
        currentIMG = frame; //storing the current img frame
    }
}

Hardware::Hardware() = default;

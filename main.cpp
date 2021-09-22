#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/**
 @brief function used to draw circles with center found in houghcircles vec3f results
 **/
void drawCircles(vector<Vec3f> &results, Mat &toMat) {

    for (auto &result: results) {
        Vec3i c = result;
        Point center = Point(c[0], c[1]);
        // circle center
        circle(toMat, center, 1, Scalar(255, 0, 255), 1, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(toMat, center, radius, Scalar(255, 0, 255), 1, LINE_AA);
    }
}

Mat clearImg(Mat &img) {
    Mat blacked_canny, bilateral;
    GaussianBlur(img, blacked_canny, Size(7, 7), 0, 0);
    bilateralFilter(blacked_canny, bilateral, 9, 80, 80);
    /*
     * SOME TEST WITH FINDCOUNTOURS IN ORDER TO CLEAR BETTER BUT WITH NO DRAMATICALLY IMPROVEMENT
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(preprocessed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    drawContours(preprocessed, contours, -1, (255, 0, 255), 0.1);
     */

    return bilateral;
}

void detectPupil(Mat &preprocessed, Mat &initialIMG) {
    Mat canny;
    Canny(preprocessed, canny, 20, 40, 3);
    //TODO: IN CALIBRATION  STORE THE  VALUES FOR THE PUPIL OF THAT SESION
    vector<Vec3f> storage;
    HoughCircles(canny, storage, HOUGH_GRADIENT, 2,
                 canny.rows / 16, 100, 80, 15, 40);
    drawCircles(storage, initialIMG);
}

void detectIRLEDS(Mat &preprocessed, Mat &initialIMG) {
    Mat canny;
    Canny(preprocessed, canny, 150, 250, 3);

    vector<Vec3f> storage;
    HoughCircles(canny, storage, HOUGH_GRADIENT, 2,
                 5, 500, 15, 1, 5);
    drawCircles(storage, initialIMG);
}

void processImage(Mat tempImg) {
    Mat preprocessed = clearImg(tempImg);
    detectPupil(preprocessed, tempImg);
    detectIRLEDS(preprocessed, tempImg);
    imshow("test", tempImg);
    if (waitKey(200) >= 0);
}

//Opens a folder with images and stores them on the images vector
void openTestImages(vector<Mat> &images, vector<Mat> &processedImages, const string &folderName) {
    vector<cv::String> fn; //Vector  for saving filenames  of folder
    string filDir = "./" + folderName + "/*.bmp";
    cv::glob(filDir, fn, true);
    //Iterating over the filenames
    for (const cv::String &filename: fn) {
        const Mat tempImg = imread(filename); //Creating the image from the filename
        processImage(tempImg); //Processing the images
    }
}

void openCamara() {
    VideoCapture cap(2);
    if (!cap.isOpened()) {
        cout << "Camara not found";
        return;
    }
    for (;;) {
        Mat frame, resized;
        cap >> frame;
        if (waitKey(50) >= 0) break;
        resize(frame, resized, Size(400, 394));
        imshow("Test", resized);
        cout << "Mat of rows: " << frame.rows << "Mat of columns" << frame.cols << endl;
    }

}

int main() {
    vector<Mat> images;
    vector<Mat> resultImages;
    openTestImages(images, resultImages, "IrisDB");
    //openCamara();
    return EXIT_SUCCESS;
}


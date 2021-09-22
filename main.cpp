#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>

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
    Mat blacked_canny, preprocessed, bilateral;
    GaussianBlur(img, blacked_canny, Size(7, 7), 0, 0);
    bilateralFilter(blacked_canny, bilateral, 9, 80, 80);
    /*
     * SOME TEST WITH FINDCOUNTOURS IN ORDER TO CLEAR BETTER BUT WITH NO DRAMATICALLY IMPROVEMENT
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(preprocessed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    drawContours(preprocessed, contours, -1, (255, 0, 255), 0.1);
     */

    return preprocessed;
}

void detectPupil(Mat &preprocessed, Mat &initialIMG) {
    vector<Vec3f> storage;
    Mat canny;
    Canny(preprocessed, canny, 20, 40, 3);
    //TODO: IN CALIBRATION  STORE THE  VALUES FOR THE PUPIL OF THAT SESION
    HoughCircles(canny, storage, HOUGH_GRADIENT, 2,
                 canny.rows / 16, 100, 80, 15, 40);
    drawCircles(storage, initialIMG);
}

void detectIRLEDS(Mat &preprocessed, Mat &initialIMG) {
    vector<Vec3f> storage;
    Canny(bilateral, preprocessed, 20, 40, 3);
    Canny(preprocessed, preprocessed, 100, 200, 3);
    //TODO: IN CALIBRATION  STORE THE  VALUES FOR THE PUPIL OF THAT SESION
    /*
    HoughCircles(preprocessed, storage, HOUGH_GRADIENT, 2,
                 preprocessed.rows / 16, 100, 80, 15, 40);
    drawCircles(storage, initialIMG);
     */
}

void processImage(vector<Mat> &processedImages, Mat tempImg) {
    Mat preprocessed = clearImg(tempImg);
    //detectPupil(preprocessed, tempImg);
    detectIRLEDS(preprocessed, tempImg);
    processedImages.push_back(preprocessed);
}

//Opens a folder with images and stores them on the images vector
void openTestImages(vector<Mat> &images, vector<Mat> &processedImages, const string &folderName) {
    vector<cv::String> fn; //Vector  for saving filenames  of folder
    string filDir = "./" + folderName + "/*.bmp";
    cv::glob(filDir, fn, true);
    //Iterating over the filenames
    for (const cv::String &filename: fn) {
        const Mat tempImg = imread(filename); //Creating the image from the filename
        images.push_back(tempImg);
        processImage(processedImages, tempImg); //Processing the images
    }
}

void showImages(vector<Mat> &images) {
    //Iterating over the test images
    const int delay = 150;
    for (Mat &img: images) {
        imshow("Sample", img);
        if (waitKey(delay) >= 0)
            break;
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
    showImages(resultImages);
    //openCamara();
    return EXIT_SUCCESS;
}


#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void processImage(vector<Mat> &processedImages, Mat tempImg) {
    // -----------------------------------
// STEP 2: find the iris outer contour
// -----------------------------------
// Detect iris outer border
// Apply a canny edge filter to look for borders
// Then clean it a bit by adding a smoothing filter, reducing noise
    Mat blacked_canny, preprocessed, bilateral;
    GaussianBlur(tempImg, blacked_canny, Size(7, 7), 0, 0);
    bilateralFilter(blacked_canny, bilateral, 9, 80, 80);
    Canny(bilateral, preprocessed, 25, 75, 3);
// Now run a set of HoughCircle detections with different parameters
// We increase the second accumulator value until a single circle is left and take that one for granted
    int i = 80;
    Vec3f found_circle;
    while (i < 151) {
        vector<Vec3f> storage;
        // If you use other data than the database provided, tweaking of these parameters will be neccesary
        HoughCircles(preprocessed, storage, HOUGH_GRADIENT, 2, 100.0, 30, i, 15, 35);
        if (storage.size() == 1) {
            found_circle = storage[0];
            break;
        }
        i++;
    }
// Now draw the outer circle of the iris
// For that we need two 3 channel BGR images, else we cannot draw in color
    Mat blacked_c(tempImg.rows, tempImg.cols, CV_8UC3);
    Mat in[] = {tempImg, tempImg, tempImg};
    int from_to[] = {0, 0, 1, 1, 2, 2};
    mixChannels(in, 3, &blacked_c, 1, from_to, 3);
    circle(blacked_c, Point(found_circle[0], found_circle[1]), found_circle[2], Scalar(0, 0, 255), 1);
    processedImages.push_back(blacked_c);
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

int main() {
    vector<Mat> images;
    vector<Mat> resultImages;
    openTestImages(images, resultImages, "IrisDB");
    //showImages(images);
    showImages(resultImages);
    return 0;
}


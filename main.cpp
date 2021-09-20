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
    Mat blacked_canny, preprocessed, bilateral;
    GaussianBlur(img, blacked_canny, Size(7, 7), 0, 0);
    bilateralFilter(blacked_canny, bilateral, 9, 80, 80);
    Canny(bilateral, preprocessed, 25, 75, 3);
    return preprocessed;
}

void processImage(vector<Mat> &processedImages, Mat tempImg) {
    Mat preprocessed = clearImg(tempImg);
    vector<Vec3f> storage;
    HoughCircles(preprocessed, storage, HOUGH_GRADIENT, 2,
                 preprocessed.rows / 16, 100, 80, 15, 40);
    drawCircles(storage, tempImg);
    processedImages.push_back(tempImg);
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
    showImages(resultImages);
    return EXIT_SUCCESS;
}


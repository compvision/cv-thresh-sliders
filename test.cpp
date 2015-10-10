#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

std::string path = "/home/agentg010/Pictures/awesomeface.png";

// Global variables to store hue channel of image and the slider values
cv::Mat hue;
int minHue = 0;
int maxHue = 0;

// Do all the image processing everytime the slider value updates
void imageProcess(int, void* nop)
{
    // Perform upper bound and lower bound thresholding
    cv::Mat threshLow;
    cv::Mat threshHigh;
    cv::threshold(hue, threshLow, minHue, 255, cv::THRESH_BINARY);
    cv::threshold(hue, threshHigh, maxHue, 255, cv::THRESH_BINARY_INV);
    // Bitwise AND the upper and lower bounds for final image
    cv::Mat threshed = threshLow & threshHigh;

    cv::imshow("threshLow", threshLow);
    cv::imshow("threshHigh", threshHigh);
    cv::imshow("Thresholded", threshed);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <imgpath>\n";
        return 1;
    }

    // Blur the image a little bit to smooth out imperfections
    cv::Mat img = cv::imread(argv[1]);
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0);

    cv::imshow("Full", img);

    // Convert image to HSV color model
    cv::Mat cvted;
    cv::cvtColor(img, cvted, CV_BGR2HSV);

    // Grab the Hue channel only
    std::vector<cv::Mat> separated(3);
    cv::split(cvted, separated);
    hue = separated.at(0).clone();
    
    // Window stuff
    cv::namedWindow("Thresholded", cv::WINDOW_NORMAL);

    cv::createTrackbar("hueMin", "Thresholded", &minHue, 360, imageProcess);
    cv::createTrackbar("hueMax", "Thresholded", &maxHue, 360, imageProcess);

    // Do the image processing once initially
    imageProcess(1, NULL);

    cv::waitKey(0);

    return 0;
}

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// Global Variables
cv::Mat hue;
int minHue = 0;
int maxHue = 0;

// Do all the thresholding
void threshold(int, void*)
{
    using namespace cv;

    Mat threshLow;
    Mat threshHigh;
    threshold(hue, threshLow, minHue, 255, THRESH_BINARY);
    threshold(hue, threshHigh, maxHue, 255, THRESH_BINARY_INV);
    Mat threshed = threshLow & threshHigh;

    imshow("threshLow", threshLow);
    imshow("threshHigh", threshHigh);
    imshow("Thresholded", threshed);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <imgpath>\n";
        return 1;
    }

    cv::Mat img = cv::imread(argv[1]);

    // Blur the image to smooth it out (especially with JPG's)
    cv::GaussianBlur(img, img, cv::Size(3, 3), 1, 1);

    cv::imshow("Full", img);

    // Convert to HSV
    cv::Mat cvted;
    cv::cvtColor(img, cvted, CV_BGR2HSV);

    // Isolate the Hue Channel, and store in global variable
    std::vector<cv::Mat> separated(3);
    cv::split(cvted, separated);
    hue = separated.at(0).clone();
    
    cv::namedWindow("Thresholded", cv::WINDOW_NORMAL);

    cv::createTrackbar("hueMin", "Thresholded", &minHue, 255, threshold);
    cv::createTrackbar("hueMax", "Thresholded", &maxHue, 255, threshold);

    // Do the image processing once initially (parameters have no significance)
    threshold(1, NULL);

    cv::waitKey(0);

    return 0;
}

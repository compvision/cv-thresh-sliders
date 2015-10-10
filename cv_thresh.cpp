#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

std::string path = "/home/agentg010/Pictures/awesomeface.png";

cv::Mat hue;
int minHue = 0;
int maxHue = 0;

// Do all the image processing
void imageProcess(int, void* nop)
{
    cv::Mat threshLow;
    cv::Mat threshHigh;
    cv::threshold(hue, threshLow, minHue, 255, cv::THRESH_BINARY);
    cv::threshold(hue, threshHigh, maxHue, 255, cv::THRESH_BINARY_INV);
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
    cv::Mat img = cv::imread(argv[1]);
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0);
    std::vector<cv::Mat> separated(3);

    cv::imshow("Full", img);

    cv::Mat cvted;
    cv::cvtColor(img, cvted, CV_BGR2HSV);

    cv::split(cvted, separated);
    hue = separated.at(0).clone();
    
    cv::namedWindow("Thresholded", cv::WINDOW_NORMAL);

    cv::createTrackbar("hueMin", "Thresholded", &minHue, 360, imageProcess);
    cv::createTrackbar("hueMax", "Thresholded", &maxHue, 360, imageProcess);

    // Do the image processing once initially
    imageProcess(1, NULL);

    cv::waitKey(0);

    return 0;
}

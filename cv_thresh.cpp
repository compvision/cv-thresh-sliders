#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;

// Global Variables
Mat hue;
Mat val;
int minHue = 0;
int maxHue = 0;
int maxVal = 0;
int minVal = 0;
std::string windowId = "CV LIVE THRESHOLDING DEMO";

// Do all the thresholding
void threshold(int, void*)
{
    using namespace cv;

    Mat threshLow;
    Mat threshHigh;
    Mat hueResult;
    Mat valResult;

//hue
    threshold(hue, threshLow, minHue, 255, THRESH_BINARY);
    threshold(hue, threshHigh, maxHue, 255, THRESH_BINARY_INV);
    hueResult = threshLow & threshHigh;    

//value
    threshold(val, threshLow, minVal, 255, THRESH_BINARY);
    threshold(val, threshHigh, maxVal, 255, THRESH_BINARY_INV);
    valResult = threshLow & threshHigh;

    Mat threshed = hueResult & valResult;

    //imshow("threshLow", threshLow);
    //imshow("threshHigh", threshHigh);
    imshow(windowId, threshed);
}
void run(Mat img) {
    // Blur the image to smooth it out (especially with JPG's)
    GaussianBlur(img, img, Size(3, 3), 1, 1);

    //imshow("Full", img);

    // Convert to HSV
    Mat cvted;
    cvtColor(img, cvted, CV_BGR2HSV);

    // Isolate the Hue Channel, and store in global variable
    std::vector<Mat> separated(3);
    split(cvted, separated);
    hue = separated.at(0).clone();
    val = separated.at(2).clone();

    namedWindow(windowId, WINDOW_NORMAL);

    createTrackbar("hueMin", windowId, &minHue, 255, threshold);
    createTrackbar("hueMax", windowId, &maxHue, 255, threshold);
    createTrackbar("valMin", windowId, &minVal, 255, threshold);
    createTrackbar("valMax", windowId, &maxVal, 255, threshold);
    imshow(windowId, img);
    // Do the image processing once initially (parameters have no significance)
    threshold(1, NULL);

    if (waitKey(30) >= 0){

    }
}

int main(int argc, char* argv[])
{
    bool error = false;
    Mat img;

    if(argc == 3) {

        std::cout << "Initializing Camera @ Index " << argv[2];
        VideoCapture cam = VideoCapture(atoi(argv[2]));

        while(true) {
            Mat img;
            cam.read(img);

            //imshow("Live", img);
            run(img);
            if (waitKey(30) >= 0){break;}
        }
    }
    else if (argc == 2){
        std::string arg1(argv[1]);
        if (arg1 != "-d") {
            std::cout << "Normal Init";
            img = imread(argv[1]);
            while(true) {
                run(img);
                if (waitKey(30) >= 0){break;}    
            }
            //run(img);
        }
        else {
            error = true;
        }
    }
    else{
        error = true;
    }
    if (error){
        std::cout << std::endl;
        std::cout << "Image Loading:\nUsage: " << argv[0] << " <imgpath>\n";
        std::cout << "\nLive Video Feed Loading:";
        std::cout << "Usage: " << argv[0] << " -d INDEX\n";
        std::cout<< std::endl;
        return 1;
    }

    return 0;
}

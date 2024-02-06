/*#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Load video
    VideoCapture video("D:\\FALL 2023\\Senior Capstone\\Media\\Video\\Fire2.mp4");

    // Check if video opened successfully
    if (!video.isOpened())
    {
        cout << "Error opening video file" << endl;
        return -1;
    }

    // Create SimpleBlobDetector object
    SimpleBlobDetector::Params params;
    // params.blobColor = 100;
    params.filterByArea = true;
    params.minArea = 5;
    params.maxArea = 300;
    params.filterByCircularity = false;
    params.minCircularity = (float)0.1;
    params.maxCircularity = (float)1.0;
    params.filterByColor = false;
    params.filterByConvexity = false;
    params.minConvexity = (float)0.1;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Loop through video frames
    Mat colorFrame, grayFrame, thresh, fireMask;
    // vector<KeyPoint> keypoints;
    vector<KeyPoint> keyPointsFire;
    bool initAreaOfScreen = true;

    //  Read in the first frame to init the area of the screen
    video.read(colorFrame);

    //  Define an area of the screen at the bottom of the screen that contains the fast moving fire line - only for this video
    //  This will be a rectangle that contains the fire line that is going from left to right in the video only

    int x1 = 0;
    int y1 = 0;
    int x2 = colorFrame.cols;
    int y2 = colorFrame.rows;

    while (video.read(colorFrame)) {

        // Convert colorFrame to grayscale
        cvtColor(colorFrame, grayFrame, COLOR_BGR2GRAY);

        // Threshold image
        // threshold(gray, thresh, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

        //// Setup for Fire with Color Frame
        //Scalar fireLower = (0, 0, 0);
        //Scalar fireUpper = (200, 200, 255);
        //
        //inRange(frame, fireLower, fireUpper, fireMask);

        // Setup for Fire with B&W Frame
        Scalar fireLower = (110);
        Scalar fireUpper = (150);

        inRange(grayFrame, fireLower, fireUpper, fireMask);

        // Detect blobs
        detector->detect(grayFrame, keyPointsFire, fireMask);

        // Draw detected blobs as red circles
        Mat frameWithKeypoints;

        for (int i = 0; i < keyPointsFire.size(); i++) {

            float x = keyPointsFire[i].pt.x;
            float y = keyPointsFire[i].pt.y;

            if ((x1 <= x) and (x <= x2) and (y1 <= y) and (y <= y2)) {

                // Draw this blob in the defined area
                // drawKeypoints(colorFrame, keyPointsFire, frameWithKeypoints, Scalar(0, 0, 255));
                circle(colorFrame, keyPointsFire[i].pt, keyPointsFire[i].size, Scalar(0, 0, 255), 1);

                // Display result
                imshow("Blob Tracking [Experimental]  Press 'ESC' to stop Video", colorFrame);

            }
            else {
                // Discard this blob - not doing anything here
            }

        }

        // Display result - moved to for loop
        // imshow("Blob Tracking [Experimental]  Press 'ESC' to stop Video", frameWithKeypoints);

        if (waitKey(1) == 27) break;

    }

    return 0;
}*/

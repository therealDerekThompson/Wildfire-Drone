/*#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//
// Function to check if BLOB is in the defined area of the screen
//

bool theBlobIsInTheDefinedArea(int x, int y, int x1, int y1, int x2, int y2) {
    if ((x1 <= x) and (x <= x2) and (y1 <= y) and (y <= y2)) {
        return true;
    }
    else {
        return false;
    }

}

//
// Method to caluclate the delta X of the Blob
//

int deltaX(vector<int> xBlobCord) {
    int i = xBlobCord.size();
    if (i > 100) return abs((xBlobCord[i - 1] - xBlobCord[i - 100]));
    return 1;
}

//
// Method to draw metrics on the image/screen
//

int drawMetricsOnScreen(Mat image, int blobCount, vector<int> xBlobCord, int fps) {
    // Define the text to be written
    string textOut1 = "Number of BLOBs = " + to_string(blobCount);
    string textOut2 = "Speed = " + to_string(deltaX(xBlobCord));
    string textOut3 = "FPS = " + to_string(fps);
    // Define the font and its properties
    int font = FONT_HERSHEY_SIMPLEX;
    double font_scale = 1;
    Scalar color = Scalar(255, 255, 255);
    int thickness = 2;

    // Define the position of the text on the image
    Point pos1(20, 100);
    Point pos2(20, 135);
    Point pos3(20, 170);

    // Write the text on the image
    putText(image, textOut1, pos1, font, font_scale, color, thickness);
    putText(image, textOut2, pos2, font, font_scale, color, thickness);
    putText(image, textOut3, pos3, font, font_scale, color, thickness);


    return 0;
}


int main(int argc, char** argv)
{
    //Create Mat frame
    Mat frame, frame1, dst;

    //Matrices
    Mat kernel = (Mat_<char>(3, 3) <<
        1, 2, 1,
        0, 0, 0,
        -1, -2, -1);

    Mat kernel1 = (Mat_<char>(3, 3) <<
        -1, 0, 1,
        2, 0, 2,
        -1, 0, 1);
    
    // Create SimpleBlobDetector object
    SimpleBlobDetector::Params params;
    // params.blobColor = 100;
    params.filterByArea = true;
    params.minArea = 1000;
    params.maxArea = 5000;
    params.filterByCircularity = false;
    params.minCircularity = (float)0.1;
    params.maxCircularity = (float)1.0;
    params.filterByColor = false;
    params.filterByConvexity = false;
    params.minConvexity = (float)0.1;

    // Open the default camera (camera index 0)
    VideoCapture video;

    int deviceID = 0;             // 0 = open default camera
    int apiID = CAP_ANY;

    video.open(deviceID + apiID);

    double fps = video.get(cv::CAP_PROP_FPS);

    // Check if the camera opened successfully
    if (!video.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    for (;;)

    //Alternative way of showing video
    Mat grey, sobelx;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        video.read(frame);
        
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        cvtColor(frame, grey, COLOR_BGR2GRAY);
        Sobel(grey, frame, CV_32F, 1, 0);

        imshow("sobel", frame);
        if (waitKey(5) >= 0)
            break;
    }

    for (;;)
    {
        video.read(frame1);

        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        filter2D(frame1, frame1, frame1.depth(), kernel);
        imshow("Filter1", frame1);
        if (waitKey(5) >= 0)
            break;
    }

    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        video.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        filter2D(frame, frame, frame.depth(), kernel1);

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }

    
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Loop through video frames
    Mat colorFrame, grayFrame, thresh, fireMask1, fireMask2, fireMaskFinal;

    // vector<KeyPoint> keypoints;
    vector<KeyPoint> keyPointsFire;

    //  Read in the first frame to init the area of the screen
    video.read(colorFrame);

    //  Define an area of the screen at the bottom of the screen that contains the fast moving fire line - only for this video
    //  This will be a rectangle that contains the fire line that is going from right to left in the video only

    int x1 = 0;
    int y1 = 0;
    int x2 = colorFrame.cols;
    int y2 = colorFrame.rows;

    // Init BLOB x-cord. vars
    int blobCount = 0;
    vector<int> xBlobCord;
    /*
    while (video.read(colorFrame)) {

        // Convert colorFrame to grayscale
        cvtColor(colorFrame, grayFrame, COLOR_BGR2GRAY);
        //filter2D(colorFrame, fireMask1, colorFrame.depth(), kernel1);

        // Threshold image
       // threshold(grayFrame, thresh, 250, 255, THRESH_BINARY_INV + THRESH_OTSU);
        threshold(grayFrame, thresh, 250, 255, 1);

        //// Setup for Fire with Color Frame
        //Scalar fireLower = (0, 0, 0);
        //Scalar fireUpper = (200, 200, 255);
        //
        //inRange(frame, fireLower, fireUpper, fireMask);

        // Setup for Fire with B&W Frame
        // Scalar fireLower1 = (200);
        // Scalar fireUpper1 = (255);
        // Scalar fireLower2 = (175);
        // Scalar fireUpper2 = (199);

        // inRange(grayFrame, fireLower1, fireUpper1, fireMask1);
        // inRange(grayFrame, fireLower2, fireUpper2, fireMask2);

        // Combine the two masks into a final mask
        // bitwise_or(fireMask1, fireMask2, fireMaskFinal);

        // Detect blobs
        // detector->detect(grayFrame, keyPointsFire, fireMaskFinal);
        detector->detect(thresh, keyPointsFire);

        // Draw detected blobs as red circles
        // Mat frameWithKeypoints;

        for (int i = 0; i < keyPointsFire.size(); i++) {

            int x = keyPointsFire[i].pt.x;
            int y = keyPointsFire[i].pt.y;

            if (true)
                // (theBlobIsInTheDefinedArea(x, y, x1, y1, x2, y2))
            {

                // Store BLOB's x-cord. to calculate delta x
                blobCount = keyPointsFire.size();
                xBlobCord.push_back(x);

                drawMetricsOnScreen(colorFrame, blobCount, xBlobCord, fps);

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
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

double calculateSpeed(vector<KeyPoint> keyPointsFire);
void drawMetricsOnScreen(Mat image, vector<KeyPoint> keyPointsFire, double fps);

int main() {
    Mat frame, dst;
	VideoCapture video;

    int deviceID = 0;             // 0 = open default camera
    int apiID = CAP_ANY;

    video.open(deviceID + apiID);

    double fps = video.get(cv::CAP_PROP_FPS);

    // Create SimpleBlobDetector Parameter Object
    SimpleBlobDetector::Params params;
    // Specify SimpleBlobDetector parameters
    params.filterByArea = true;
    params.minArea = 1000;
    params.maxArea = 5000;
    params.filterByCircularity = false;
    params.minCircularity = (float)0.1;
    params.maxCircularity = (float)1.0;
    params.filterByColor = false;
    params.filterByConvexity = false;
    params.minConvexity = (float)0.1;
    //Create SimpleBlobDetector Object according to specified parameters
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    
    // vector<KeyPoint> keypoints;
    vector<KeyPoint> keyPointsFire;

    // Check if the camera opened successfully
    if (!video.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

	for (;;)
	{
        video.read(frame);
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        cvtColor(frame, dst, COLOR_BGR2GRAY);
        detector->detect(dst, keyPointsFire);

        int n = keyPointsFire.size();

        
        circle(frame, keyPointsFire[n - 1].pt, keyPointsFire[n - 1].size, Scalar(0, 0, 255), 7);
        
        

        drawMetricsOnScreen(frame, keyPointsFire, fps);

        imshow("Thresholding", frame);
        if (waitKey(5) >= 0)
            break;
	}

    return 0;
}

//Additional Methods
void drawMetricsOnScreen(Mat image, vector<KeyPoint> keyPointsFire, double fps) {


    // Define the text to be written
    string textOut1 = "Number of BLOBs = " + to_string(keyPointsFire.size());
    string textOut2 = "Speed = " + to_string(calculateSpeed(keyPointsFire));
    string textOut3 = "FPS = " + to_string(24);
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

}

double calculateSpeed(vector<KeyPoint> keyPointsFire)
{
    int n = keyPointsFire.size();
    if (keyPointsFire.size() >= 2)
    {
        cv::Point2f endpoint = keyPointsFire[n - 1].pt;
        cv::Point2f startpoint = keyPointsFire[n - 2].pt;
        double speed = (((endpoint.y) - (startpoint.y)) / (endpoint.x - startpoint.x));
        return speed;
    }
    else
        return 0.0;
}
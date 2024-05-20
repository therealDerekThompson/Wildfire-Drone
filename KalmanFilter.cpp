#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

double calculateSpeed(vector<KeyPoint> keyPointsFire);
Point calculateFutureLocation(Mat state);
void drawMetricsOnScreen(Mat image, vector<KeyPoint> keyPointsFire, double fps);

int main() {
    Mat frame, dst, prediction, state;
    bool found = false;
    VideoCapture video("C:\\Users\\admin\\Documents\\University\\FALL 2023\\Senior Capstone\\Media\\Video\\Demo2.mp4");

    namedWindow("Display", WINDOW_NORMAL);

    // Check if the camera opened successfully
    if (!video.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    double fps = 10;

    // Create SimpleBlobDetector Parameter Object
    SimpleBlobDetector::Params params;
    // Specify SimpleBlobDetector parameters
    params.filterByArea = true;
    params.minArea = 30;
    params.filterByCircularity = false;
    params.minCircularity = (float)0.1;
    params.maxCircularity = (float)1.0;
    params.filterByColor = false;
    params.filterByConvexity = false;
    params.minConvexity = (float)0.1;
    params.minThreshold = 190;
    params.maxThreshold = 255;
    params.thresholdStep = 13;
    //Create SimpleBlobDetector Object according to specified parameters
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // vector<KeyPoint> keypoints;
    KeyPoint dummy(1, 1, 1, 1);
    vector<KeyPoint> keyPointsFire;
    keyPointsFire.push_back(dummy);
    state = (Mat_<float>(4, 1) <<   1,
                                    1,
                                    1,
                                    1);

    //Create Kalman Filter
    KalmanFilter kalmanFilter(4, 2);
    kalmanFilter.measurementMatrix = (Mat_<float>(2, 4) << 1, 0, 0, 0,
                                                           0, 1, 0, 0);

    //Transition model based of simple linear motion model
    kalmanFilter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0,
                                                          0, 1, 0, 1,
                                                          0, 0, 1, 0,
                                                          0, 0, 0, 1);

    //Process noise covariance matrix adds small errors to our 4 state variables
    kalmanFilter.processNoiseCov = (Mat_<float>(4, 4) << 1, 0, 1, 0,
                                                         0, 1, 0, 1,
                                                         0, 0, 1, 0,
                                                         0, 0, 0, 1) * 25;


    //Get center of video frame for initial state
    float cx = video.get(CAP_PROP_FRAME_WIDTH) / 2;
    float cy = video.get(CAP_PROP_FRAME_HEIGHT) / 2;
    kalmanFilter.statePost = (Mat_<float>(4, 1) << cx,
                                                   cy,
                                                   0,
                                                   0);

    kalmanFilter.statePre = kalmanFilter.statePost;
    //Initial observation 
    kalmanFilter.temp1 = (Mat_<float>(2, 1) << 0,
                                               0);

    kalmanFilter.temp2 = (Mat_<float>(2, 1) << 0,
                                               0);

    //Create Initial Values for KeyPointsFire so we don't have to change values at end of loop

    double ticks = 0;
    double execution_ticks = 0;

    for (;;)
    {

        double execution_ticks = (double)getTickCount();

        video.read(frame);
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        double precTick = ticks;
        ticks = (double)getTickCount();
        double dT = (ticks - precTick) / getTickFrequency(); //seconds
        
        cvtColor(frame, dst, COLOR_BGR2GRAY);
        detector->detect(dst, keyPointsFire);

        int n = keyPointsFire.size();

        Mat prediction = kalmanFilter.predict();
        Point predictedPoint(prediction.at<float>(0), prediction.at<float>(1));
        

        if (n > 0) {
            //Draw Fire location
            circle(frame, keyPointsFire[n - 1].pt, keyPointsFire[n - 1].size, Scalar(0, 0, 255), 7);

            //Project the state ahead
            
            circle(frame, predictedPoint, 15, Scalar(0, 255, 0), 7);

            //Store location in case of blank
            kalmanFilter.temp2 = (Mat_<float>(2, 1) << keyPointsFire[n - 1].pt.x,
                                                       keyPointsFire[n - 1].pt.y);;

            //Reset matrices for new batch of keypoints
            
            if (!found) {
                // First detection
                kalmanFilter.statePre.at<float>(0) = keyPointsFire[0].pt.x;
                kalmanFilter.statePre.at<float>(1) = keyPointsFire[0].pt.y;
                kalmanFilter.statePre.at<float>(2) = 0;
                kalmanFilter.statePre.at<float>(3) = 0;

                kalmanFilter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0,
                                                                      0, 1, 0, 1,
                                                                      0, 0, 1, 0,
                                                                      0, 0, 0, 1);
                setIdentity(kalmanFilter.measurementMatrix);
                setIdentity(kalmanFilter.processNoiseCov, Scalar::all(1e-4));
                setIdentity(kalmanFilter.measurementNoiseCov, Scalar::all(1e-1));
                setIdentity(kalmanFilter.errorCovPost, Scalar::all(1));
                found = true;
            }

            //Update observation
            kalmanFilter.temp1 = (Mat_<float>(2, 1) << keyPointsFire[n - 1].pt.x,
                                                       keyPointsFire[n - 1].pt.y);
            
            kalmanFilter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, dT, 0,
                                                                  0, 1, 0, dT,
                                                                  0, 0, 1, 0,
                                                                  0, 0, 0, 1);


            state = kalmanFilter.correct(kalmanFilter.temp1);

            cout << "Prediction: " << endl << state << endl;

            Point correctedPoint(state.at<float>(0), state.at<float>(1));

            line(frame, predictedPoint, correctedPoint, Scalar(0, 0, 0), 4);

        }
        ;
        kalmanFilter.statePre.at<float>(0) = keyPointsFire[0].pt.x;
        kalmanFilter.statePre.at<float>(1) = keyPointsFire[0].pt.y;
        kalmanFilter.statePre.at<float>(2) = 0;
        kalmanFilter.statePre.at<float>(3) = 0;
        setIdentity(kalmanFilter.measurementMatrix);
        setIdentity(kalmanFilter.processNoiseCov, Scalar::all(1e-4));
        setIdentity(kalmanFilter.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(kalmanFilter.errorCovPost, Scalar::all(1));

        cout << "Prediction: " << endl << state << endl;

        //Project the state ahead

        circle(frame, predictedPoint, 30, Scalar(255, 0, 0), 7);

        Point correctedPoint(state.at<float>(0), state.at<float>(1));

        line(frame, predictedPoint, correctedPoint, Scalar(0, 0, 0), 4);
        
        kalmanFilter.statePre = kalmanFilter.statePost;
        
        drawMetricsOnScreen(frame, keyPointsFire, fps);

        

        imshow("Display", frame);
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

Point calculateFutureLocation(Mat state) {
    double x = state.at<float>(0) + state.at<float>(2);
    double y = state.at<float>(1) + state.at<float>(3);
    Point calculatedPoint(x, y);
    return calculatedPoint;
}

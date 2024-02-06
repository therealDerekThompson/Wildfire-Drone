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
// Method to draw metrics on the image/screen
//
int drawMetricsOnScreen(Mat image, int blobCount) {
	// Define the text to be written

	string textOut = "Number of BLOBs: " + to_string(blobCount);
	// Define the font and its properties

	int font = FONT_HERSHEY_SIMPLEX;
	double font_scale = 1;
	Scalar color = Scalar(255, 255, 255);

	int thickness = 2;
	// Define the position of the text on the image

	Point pos(20, 100);
	// Write the text on the image

	putText(image, textOut, pos, font, font_scale, color, thickness);

	return 0;
}
int main(int argc, char** argv)
{
	// Load video
	VideoCapture video("D:\\FALL 2023\\Senior Capstone\\Media\\Video\\fire_for_the_group.mp4");
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

	params.minArea = 200;

	params.maxArea = 600;

	params.filterByCircularity = false;

	params.minCircularity = (float)0.1;

	params.maxCircularity = (float)1.0;

	params.filterByColor = false;

	params.filterByConvexity = false;

	params.minConvexity = (float)0.1;


	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	// Loop through video frames
	Mat colorFrame, grayFrame, thresh, fireMask1, fireMask2, fireMaskFinal;
	// vector<KeyPoint> keypoints;
	vector<KeyPoint> keyPointsFire;

	// Read in the first frame to init the area of the screen
	video.read(colorFrame);
	// Define an area of the screen at the bottom of the screen that contains the fast moving fire line - only for this video
	// This will be a rectangle that contains the fire line that is going from right to left in the video only


	int x1 = 0;

	int y1 = 0;

	int x2 = colorFrame.cols;

	int y2 = colorFrame.rows;
	// Init BLOB x-cord. vars

	int blobCount = 0;
	vector<int> xBlobCord;
	while (video.read(colorFrame)) {


		// Convert colorFrame to grayscale

		cvtColor(colorFrame, grayFrame, COLOR_BGR2GRAY);

		// Threshold image
		//HOG Classifier
		//threshold(gray, thresh, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

		//// Setup for Fire with Color Frame

		//Scalar fireLower = (0, 0, 0);

		//Scalar fireUpper = (200, 200, 255);

		//

		//inRange(frame, fireLower, fireUpper, fireMask);

		// Setup for Fire with B&W Frame
		Scalar fireLower1 = (230);

		Scalar fireUpper1 = (255);

		Scalar fireLower2 = (55);

		Scalar fireUpper2 = (60);

		inRange(grayFrame, fireLower1, fireUpper1, fireMask1);

		inRange(grayFrame, fireLower2, fireUpper2, fireMask2);

		// Combine the two masks into a final mask

		bitwise_or(fireMask1, fireMask2, fireMaskFinal);


		// Detect blobs

		detector->detect(grayFrame, keyPointsFire, fireMaskFinal);


		// Draw detected blobs as red circles

		// Mat frameWithKeypoints;

		for (int i = 0; i < keyPointsFire.size(); i++) {

			int x = keyPointsFire[i].pt.x;

			int y = keyPointsFire[i].pt.y;


			if (theBlobIsInTheDefinedArea(x, y, x1, y1, x2, y2)) {

				// Store BLOB's x-cord. to calculate delta x

				blobCount = keyPointsFire.size();

				xBlobCord.push_back((int)x);

				drawMetricsOnScreen(colorFrame, blobCount);

				// Draw this blob in the defined area

				// drawKeypoints(colorFrame, keyPointsFire, frameWithKeypoints, Scalar(0, 0, 255));

				circle(colorFrame, keyPointsFire[i].pt, keyPointsFire[i].size, Scalar(0, 0, 255), 1);


				// Display result

				imshow("Blob Tracking [Experimental] Press 'ESC' to stop Video", colorFrame);


			}

			else {

				// Discard this blob - not doing anything here

			}

		}

		// Display result - moved to for loop

		// imshow("Blob Tracking [Experimental] Press 'ESC' to stop Video", frameWithKeypoints);


		if (waitKey(1) == 27) break;

	}

	return 0;
}*/
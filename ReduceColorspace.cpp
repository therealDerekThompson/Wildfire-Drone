/*#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat& ScanImageAndReduceC(Mat& I, const uchar* table);

int main()
{
    int divideWith;
    cout << "Please enter a value to reduce the colorspace by: " << endl;
    cin >> divideWith;
    //string image_path = "";

    //! [imread]
    string image_path =
        "C:\\Users\\admin\\Documents\\University\\FALL 2023\\Senior Capstone\\Code\\Test_Images\\valley.jpg";
    Mat img = imread(image_path, IMREAD_UNCHANGED);
    //! [imread]

    imshow("Original", img);
    int k = waitKey(0); // Wait for a keystroke in the window

    //Generate Lookup Table
    uchar table[256];
    for (int i = 0; i < 256; ++i)
    {
        table[i] = (uchar)(divideWith * (i / divideWith));//Note that the quotient (i / divideWith) will be truncated to 0 for the first n elements where n = divideWith
    }

    //Manually
    Mat img2 = ScanImageAndReduceC(img, table);
    imshow("Reduced Color Space", img2);
    k = waitKey(0);

    //Using LUT Function
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = table[i];

    LUT(img, lookUpTable, img);
    imshow("reduced colorspace", img);
}

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();

    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i, j;
    uchar* p;
    for (i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for (j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]];
        }
    }
    return I;
}*/
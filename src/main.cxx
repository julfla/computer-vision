#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "kmeans.h"

using namespace cv;
using namespace std;



int main ( int argc, char **argv )
{
    if(argc < 2) {
        cout << "Invalid Usage" << endl;
        cout << "USE : k-means image_path" <<endl;
        return -1;
    }
    Mat3b img = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    if(!img.data) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat3b img_result = img.clone();

    KMeans kmeans(&img);
    img_result = kmeans.computeClusterization(4);

    //namedWindow( "Input Picture", CV_WINDOW_AUTOSIZE );
    //imshow( "Input Picture", img );
    //namedWindow( "Result Image", CV_WINDOW_AUTOSIZE );
    //imshow( "Result Image", img_result );
    waitKey();

    img.release();

    return 0;
}

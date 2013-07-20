#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "kmeans.h"

#include <boost/program_options.hpp> //used to parse command lne arguments
namespace po = boost::program_options;

using namespace cv;
using namespace std;


int main ( int argc, char **argv )
{
    string input_path, output_path;
    int n_cluster;
    double distance_factor;
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("factor,f", po::value<double>(&distance_factor), "factor f := distance color / distance euclidian.   default : 1.0")
            ("cluster,n", po::value<int>(&n_cluster), "Number of clusters")
            ("input,i", po::value<string>(), "Input image file")
            ("output,o", po::value<string>(), "Output image file")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (vm.count("input") && vm.count("output")) {
        output_path = vm["output"].as<string>();
        input_path = vm["input"].as<string>();
    } else {
        cout << "Input/Output file unknown or invalid." << endl;
        cout << desc << endl;
        return 1;
    }

    if(n_cluster == 0) {
        cout << "0 is not a valid number of clusters." << endl;
        cout << desc << endl;
        return 1;
    }

    if(distance_factor == 0.0){
        distance_factor = 1;
        return 1;
    }

    Mat3b img = imread(input_path, CV_LOAD_IMAGE_UNCHANGED);
    if(!img.data) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    KMeans kmeans(&img);
    Mat1b img_result = *kmeans.computeClusterization(n_cluster, distance_factor);

    namedWindow( "Input Picture", CV_WINDOW_AUTOSIZE );
    imshow( "Input Picture", img );
    namedWindow( "Result Image", CV_WINDOW_AUTOSIZE );
    imshow( "Result Image", img_result );

    while( 'q' != (char) waitKey() ){}

    img.release();
    img_result.release();

    return 0;
}

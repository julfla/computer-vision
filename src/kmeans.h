#ifndef KMEANS_H
#define KMEANS_H

#include "cv.h"
#include <iostream>
#include <boost/random.hpp>

//typedef it_3D std::iterator<cv::Point3d>;

using namespace cv;

class KMeans {

    class Cluster {

    public:

        uchar* color;
        int channels;
        Point2i center;

        Cluster(Point2i center, uchar* color) {
            this->center = center;
            this->color = color;
        }

        Cluster(Mat const *img, int x, int y) {
            center = Point2i(x,y);
            channels = img->channels();
            color = new uchar[channels];
            for(int i = 0; i < channels; ++i)
                color[i] = img->at(x,y)[i];
        }

        double distance2pixel(Mat const *img, int x, int y) {
            //TODO
            return 0;
        }
    };

    typedef std::vector<Cluster>::iterator it_2d;

    std::vector<Cluster> clusters;
    const Mat *input_image;

    void init_clusters(int n_clusters) {
        //generator for random number
        boost::minstd_rand g(std::time(0));
        boost::uniform_int<> uni_with(0,input_image->rows);
        boost::uniform_int<> uni_height(0,input_image->cols);
        //initialize the cluster centered on random position
        for(int i = 0; i < n_clusters; ++i)
            clusters.push_back(Cluster(input_image,uni_with(g),uni_height(g)));
    }

    void refresh_cluster_position() {

    }

public:

    KMeans(Mat const *input_image) {
        this->input_image = input_image;
    }

    cv::Mat computeClusterization(int n_clusters) {
        init_clusters(n_clusters);
        //TODO
        for(it_2d it = clusters.begin(); it < clusters.end(); ++it) {
            std::cout << "x:" << it->center.x << " y:" << it->center.y << std::endl;
        }
        return input_image->clone();
    }

};

#endif // KMEANS_H

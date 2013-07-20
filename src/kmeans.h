#ifndef KMEANS_H
#define KMEANS_H

#include "cv.h"
#include <iostream>
#include <boost/random.hpp>

using namespace cv;

class KMeans {

    class Cluster {

    public:

        Vec3b color;
        Vec2b center;
        double factor_distance;

        void setColor(Vec3b color) {
            this->color = color;
        }

        void setCenter(Vec2b center) {
            this->center = center;
        }

        Cluster(const Mat3b *img, int x, int y, double factor_distance) {
            this->factor_distance = factor_distance;
            center = Vec2b(x,y);
            color = img->operator ()(Point(x,y));
        }

        double distance2pixel(Vec3b pixel, int x, int y) {
            double distEucli = sqrt(pow(center[0] - x,2) + pow(center[1] - y,2));
            double distColor = 0;
            for(int i = 0; i < 3; ++i)
                distColor += pow(color.val[i] - pixel[i],2);
            distColor = sqrt(distColor);
            return factor_distance * distEucli + distColor;
        }
    };

    typedef std::vector<Cluster>::iterator it_2d;
    typedef Mat1b::iterator it_1d;

    std::vector<Cluster> clusters;
    const Mat3b *input_image;
    Mat1b* cluster_image;

    void init_clusters(int n_clusters, double factor_distance) {
        //generator for random number
        boost::minstd_rand g(std::time(0));
        boost::uniform_int<> uni_with(0,input_image->size().width);
        boost::uniform_int<> uni_height(0,input_image->size().height);
        //initialize the cluster centered on random position
        for(int i = 0; i < n_clusters; ++i)
            clusters.push_back(Cluster(input_image,uni_with(g),uni_height(g), factor_distance));
    }

    void assign_clusters() {
        for(int i = 0; i < cluster_image->size().height; ++i)
            for(int j = 0; j < cluster_image->size().width; ++j) {
                double min_dist = 1e14;
                uchar min_cluster;
                for(int k = 0; k < clusters.size(); ++k) {
                    double dist = clusters.at(k).distance2pixel(input_image->at<Vec3b>(i,j), i, j);
                    if(dist < min_dist) {
                        min_dist = dist;
                        min_cluster = k;
                    }
                }
                cluster_image->at<uchar>(i,j) = min_cluster;
            }
    }

    void update_cluster_position() {
        for(int k = 0; k < clusters.size(); ++k) {
        int number_pixel = 0;
        Vec2i center(0,0);
        Vec3i color(0,0,0);
        for(int i = 0; i < input_image->size().height; ++i)
            for(int j = 0; j < input_image->size().width; ++j)
                    if(cluster_image->at<uchar>(i,j) == k) {
                        center += Vec2b(i,j);
                        color += input_image->at<Vec3b>(i,j);
                        number_pixel++;
                    }
        clusters.at(k).setCenter(center / number_pixel);
        clusters.at(k).setColor(color / number_pixel);
        }
    }

public:

    KMeans(const Mat3b *input_image) {
        this->input_image = input_image;
        cluster_image = new Mat1b ( Mat::zeros(input_image->size(),CV_8UC1) );
    }

    cv::Mat1b* computeClusterization(int n_clusters, double factor_distance) {
        init_clusters(n_clusters, factor_distance);
        for(int i = 0; i < 10; ++i) {
            for(it_2d it = clusters.begin(); it < clusters.end(); ++it)
            assign_clusters();
            update_cluster_position();
        }
//        for(it_2d it = clusters.begin(); it < clusters.end(); ++it) {
//            std::cout << "x:" << (int) it->center[0] << " y:" << (int) it->center[1] << std::endl;
//        }
        for(it_1d it = cluster_image->begin(); it < cluster_image->end(); ++it) {
            *it *= (255 / n_clusters);
        }
        return cluster_image;
    }

    ~KMeans() {
        cluster_image->release();
    }
};

#endif // KMEANS_H

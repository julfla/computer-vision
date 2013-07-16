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

        Vec3b color;
        Vec2b center;

        void setColor(Vec3b color) {
            this->color = color;
        }

        void setCenter(Vec2b center) {
            this->center = center;
        }


        Cluster(Vec2b center, Vec3b color) {
            this->center = center;
            this->color = color;
        }

        Cluster(const Mat3b *img, int x, int y) {
            center = Vec2b(x,y);
            color = img->operator ()(Point(x,y));
        }

        double distance2pixel(Vec3b pixel, int x, int y) {
            double distEucli = sqrt(pow(center[0] - x,2) + pow(center[1] - y,2));
            double distColor = 0;
            for(int i = 0; i < 3; ++i)
                distColor += pow(color.val[i] - pixel[i],2);
            distColor = sqrt(distColor);
            return distEucli + distColor;
        }
    };

    typedef std::vector<Cluster>::iterator it_2d;

    std::vector<Cluster> clusters;
    const Mat3b *input_image;
    Mat1b* cluster_image;

    void init_clusters(int n_clusters) {
        //generator for random number
        boost::minstd_rand g(std::time(0));
        boost::uniform_int<> uni_with(0,input_image->rows);
        boost::uniform_int<> uni_height(0,input_image->cols);
        //initialize the cluster centered on random position
        for(int i = 0; i < n_clusters; ++i)
            clusters.push_back(Cluster(input_image,uni_with(g),uni_height(g)));
    }

    void assign_clusters() {
        for(int i = 0; i < input_image->rows; ++i)
            for(int j = 0; j < input_image->cols; ++j) {
                double min_dist;
                int min_cluster;
                for(int k = 0; k < clusters.size(); ++k) {
                    double dist = clusters.at(k).distance2pixel(input_image->operator ()(Point(i,j)), i, j);
                    if(dist < min_dist) {
                        min_dist = dist;
                        min_cluster = k;
                    }
                }
                cluster_image->operator ()(i,j) = min_cluster;
            }
    }

    void update_cluster_position() {
        for(int k = 0; k < clusters.size(); ++k) {
        int number_pixel = 0;
        Vec2b center(0,0);
        Vec3b color(0,0,0);
        for(int i = 0; i < input_image->rows; ++i)
                for(int j = 0; j < input_image->cols; ++j)
                    if(cluster_image->operator ()(i,j) == k) {
                        center += Vec2b(i,j);
                        color += input_image->operator ()(i,j);
                        number_pixel++;
                    }
        clusters.at(k).setCenter(center / number_pixel);
        clusters.at(k).setColor(color / number_pixel);
        }
    }

public:

    KMeans(const Mat3b *input_image) {
        this->input_image = input_image;
        cluster_image = new Mat1b(input_image->rows, input_image->cols, 1);
    }

    cv::Mat3b computeClusterization(int n_clusters) {
        init_clusters(n_clusters);
        for(int i = 0; i < 10; ++i) {
            for(it_2d it = clusters.begin(); it < clusters.end(); ++it)
                std::cout << "x:" << (int) it->center[0] << " y:" << (int) it->center[1] << std::endl;
            assign_clusters();
            update_cluster_position();
        }
        for(it_2d it = clusters.begin(); it < clusters.end(); ++it) {
            std::cout << "x:" << (int) it->center[0] << " y:" << (int) it->center[1] << std::endl;
        }
        return input_image->clone();
    }

    ~KMeans() {
        cluster_image->release();
    }
};

#endif // KMEANS_H

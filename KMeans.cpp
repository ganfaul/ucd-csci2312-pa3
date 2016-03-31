//
// Created by Gannon Faul on 3/28/16.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cassert>

#include "KMeans.h"
#include "Exceptions.h"

namespace Clustering {
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        if(k != 0) {
            __iFileName = filename;
            std::ifstream file(__iFileName);
            if(__iFileName == "") {
                throw DataFileOpenEx(__iFileName);
            }
            if (file) {
                __dimensionality = dim;
                __k = k;
                __maxIter = maxIter;
                __numNonempty = 1;
                __clusters = new Cluster *[__k];
                for (int i = 0; i < __k; i++) {
                    __clusters[i] = new Cluster(__dimensionality);
                }
                file >> *(__clusters[0]);
                file.close();
                __initCentroids = new Point *[__k];
                for (int i = 0; i < __k; i++) {
                    __initCentroids[i] = new Point(__dimensionality);
                }
                __clusters[0]->pickCentroids(__k, __initCentroids);
            } else {
                throw DataFileOpenEx(__iFileName);
            }
        } else {
            throw ZeroClustersEx();
        }
    }

    KMeans::~KMeans() {
        for (int i = 0; i < __k; i++) {
            delete __initCentroids[i];
            delete __clusters[i];
        }
        delete [] __initCentroids;
        delete [] __clusters;
    }

    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {
        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

    Cluster &KMeans::operator[](unsigned int u) {
        return *(__clusters[u]);
    }

    const Cluster &KMeans::operator[](unsigned int u) const {
        return *(__clusters[u]);
    }

    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {
        for (int i = 0; i < kmeans.__k; i++) {
            os << kmeans[i];
        }
        return os;
    }

    void KMeans::run() {
    }
}
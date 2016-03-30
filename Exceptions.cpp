//
// Created by Gannon Faul on 3/28/16.
//


#include "Exceptions.h"

using std::string;
using std::ostream;
using std::endl;

namespace Clustering {
    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __name = "OutOfBoundsEx";
        __current = c;
        __rhs = r;
    }

    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }

    int OutOfBoundsEx::getRhs() const {
        return __rhs;
    }

    string OutOfBoundsEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const OutOfBoundsEx &ex) {
        os << ex.__name << ": " << ex.__current << " : " << ex.__rhs << endl;
        return os;
    }


    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __name = "DimensionalityMismatchEx";
        __current = c;
        __rhs = r;
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

    string DimensionalityMismatchEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const DimensionalityMismatchEx &ex) {
        os << ex.__name << ": " << ex.__current << " : " << ex.__rhs << endl;
        return os;
    }


    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEx";
    }

    string ZeroClustersEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const ZeroClustersEx &ex) {
        os << ex.__name << endl;
        return os;
    }


    DataFileOpenEx::DataFileOpenEx(string filename) {
        __name = "DataFileOpenEx";
        __filename = filename;
    }

    string DataFileOpenEx::getFilename() const {
        return __filename;
    }

    string DataFileOpenEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const DataFileOpenEx &ex) {
        os << ex.__name << ": " << ex.__filename << endl;
        return os;
    }


    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }

    string ZeroDimensionsEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const ZeroDimensionsEx &ex) {
        os << ex.__name << endl;
        return os;
    }


    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }

    string EmptyClusterEx::getName() const {
        return __name;
    }

    ostream &operator<<(ostream &os, const EmptyClusterEx &ex) {
        os << ex.__name << endl;
        return os;
    }


}
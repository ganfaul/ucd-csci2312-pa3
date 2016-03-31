//
// Created by Gannon Faul on 3/25/16.
//

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

using std::sqrt;
using std::pow;

#include"Point.h"
#include"Exceptions.h"

namespace Clustering {


    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

    // ************
    // CONSTRUCTORS
    // ************

    // Default Constructor
    Point::Point(unsigned int dimensions) {
        if(dimensions == 0) {
            throw ZeroDimensionsEx();
        }
        __dim = dimensions;
        __id = __idGen++;
        __values = new double[__dim];
        for (int i = 0; i < dimensions; i++) {
            __values[i] = 0.0;
        }
    }

    // Constructor with initial values
    Point::Point(unsigned int dimensions, double *values) {
        __dim = dimensions;
        __id = __idGen++;
        __values = new double[__dim];
        for (int i = 0; i < dimensions; i++) {
            __values[i] = values[i];
        }
    }


    // *********
    // BIG THREE
    // *********

    // Copy Constructor
    Point::Point(const Point &p) {

        if (&p != this) {
            __dim = p.__dim;
            __id = p.__id;
            __values = new double[__dim];
            for (int i = 0; i < p.getDims(); i++) {
                __values[i] = p.getValue(i);
            }
        }
    }

    // Operator Overload=
    Point &Point::operator=(const Point &p) {
        if (__dim == p.__dim) {
            if (&p != this) {
                __dim = p.__dim;
                __id = p.__id;
                if (__values != NULL) {
                    delete[] __values;
                }
                __values = new double[__dim];
                for (int i = 0; i < p.getDims(); i++) {
                    __values[i] = p.getValue(i);
                }
                return *this;
            } else {
                return *this;
            }
        } else {
            throw DimensionalityMismatchEx(__dim, p.__dim);
        }
    }

    // Destructor
    Point::~Point() {
        if (__values) {
            delete[] __values;
            __values = nullptr;
        }
    }

    void Point::rewindIdGen() {
        --__idGen;
    }

    // ****************
    // MEMBER FUNCTIONS
    // ****************

    // Accessors & mutators
    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    void Point::setValue(unsigned int index, double newValue) {
        if (index < __dim) {
            __values[index] = newValue;
        } else {
            throw OutOfBoundsEx(__dim, index);
        }
    }

    double Point::getValue(unsigned int index) const {
        if (index < __dim) {
            return __values[index];
        } else {
            throw OutOfBoundsEx(__dim, index);
        }
    }

    // Distance Function
    double Point::distanceTo(const Point &p) const {

        double distance = 0.0;

        if (__dim != p.getDims()) {
            throw DimensionalityMismatchEx(__dim, p.__dim);
        } else {
            for (int i = 0; i < __dim; i++) {
                distance += pow((p.getValue(i) - __values[i]), 2);
            }

            distance = sqrt(distance);
            return distance;
        }
    }


    // Overloaded operators

    //Members
    Point &Point::operator*=(double factor) { // p *= 6; p.operator*=(6);
        for (int i = 0; i < __dim; i++) {
            __values[i] *= factor;
        }
        return *this;
    }

    Point &Point::operator/=(double divisor) {
        if (divisor != 0) {
            for (int i = 0; i < __dim; i++) {
                __values[i] /= divisor;
            }
        }
        return *this;
    }

    const Point Point::operator*(double factor) const { // prevent (p1 * 2) = p2;
        Point p(*this);
        p *= factor;
        return p;
    }

    const Point Point::operator/(double divisor) const { // p3 = p2 / 2;
        Point p(*this);
        p /= divisor;
        return p;
    }

    double &Point::operator[](unsigned int index) {
        if (index < __dim) {
            return __values[index];
        } else {
            throw OutOfBoundsEx(__dim, index);
        }
    }

    const double &Point::operator[](unsigned int index) const {
        if (index < __dim) {
            return __values[index];
        } else {
            throw OutOfBoundsEx(__dim, index);
        }
    }

    // Friends
    Point &operator+=(Point &lhs, const Point &rhs) {
        if (&lhs != &rhs) {
            if (lhs.__dim == rhs.__dim) {
                for (int i = 0; i < lhs.__dim; i++)
                    lhs.__values[i] += rhs.__values[i];
            } else {
                throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
            }
        } else {
            return lhs *= 2;
        }
        return lhs;
    }

    Point &operator-=(Point &lhs, const Point &rhs) {
        if (&lhs != &rhs) {
            if (lhs.__dim == rhs.__dim) {
                for (int i = 0; i < lhs.__dim; i++)
                    lhs.__values[i] -= rhs.__values[i];
            } else {
                throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
            }
        } else {
            return lhs /= 2;
        }
        return lhs;
    }

    const Point operator+(const Point &lhs, const Point &rhs) {
        Point p(lhs);
        p += rhs;
        return p;
    }

    const Point operator-(const Point &lhs, const Point &rhs) {
        Point p(lhs);
        p -= rhs;
        return p;
    }


    bool operator==(const Point &lhs, const Point &rhs) {
        if (lhs.__dim != rhs.__dim) {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        if (lhs.getId() == rhs.getId()) {
            if (lhs.getDims() == rhs.getDims()) {
                for (int i = 0; i < lhs.getDims(); i++) {
                    if (lhs.getValue(i) != rhs.getValue(i)) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool operator!=(const Point &lhs, const Point &rhs) {
        return !(lhs == rhs);
    }


    bool operator<(const Point &lhs, const Point &rhs) {
        if (lhs != rhs) {
            if (lhs.getDims() == rhs.getDims()) {
                for (int i = 0; i < lhs.getDims(); i++) {
                    if (lhs.getValue(i) != rhs.getValue(i)) {
                        return (lhs.getValue(i) < rhs.getValue(i));
                    }
                }
            } else {
                throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
            }
        }
        return false;
    }

    bool operator>(const Point &lhs, const Point &rhs) {
        return (rhs < lhs);
    }

    bool operator<=(const Point &lhs, const Point &rhs) {
        if (lhs < rhs) {
            return true;
        } else if (&lhs != &rhs) {
            if (lhs.getDims() == rhs.getDims()) {
                for (int i = 0; i < lhs.getDims(); i++) {
                    if (lhs.getValue(i) != rhs.getValue(i)) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        return true;
    }

    bool operator>=(const Point &lhs, const Point &rhs) {
        return (rhs <= lhs);
    }

    std::ostream &operator<<(std::ostream &out, const Point &p) {
        out << p.getValue(0);
        for (int i = 1; i < p.getDims(); i++) {
            out << ", " << p.getValue(i);
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, Point &p) {
        std::string string1;
        std::getline(in, string1);
        std::stringstream stream1(string1);
        int dim1 = std::count(string1.begin(), string1.end(), p.POINT_VALUE_DELIM);
        dim1++;
        if (p.getDims() == dim1){
            for (int i = 0; i < p.getDims(); i++) {
                std::string pointVal;
                std::getline(stream1, pointVal, ',');
                std::stringstream stream2(pointVal);
                stream2 >> p.__values[i];
            }
        } else {
            throw DimensionalityMismatchEx(p.getDims(), dim1);
        }
        return in;
    }

}
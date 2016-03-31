//
// Created by Gannon Faul on 3/25/16.
//

#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

#include "Cluster.h"
#include "Exceptions.h"

namespace Clustering {

    typedef Point * PointPtr;

    unsigned int Cluster::__idGenerator = 0;
    const char POINT_CLUSTER_ID_DELIM = ':';

    LNode::LNode(const Point & p, LNodePtr n) : point(p), next(n){ }

    void Cluster::__del() { }

    void Cluster::__cpy(LNodePtr pts) { }

    //bool Cluster::__in(const Point &p) const { }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __c(c), __p(d) {
        __dimensions = d;
        if (__c.__size != 0) {
            return;
        } else {
            toInfinity();
            __valid = false;
        }
    }

    const Point Cluster::Centroid::get() const {
        return __p;
    }

    void Cluster::Centroid::set(const Point &p) {
        __p = p;
        __valid = true;
    }

    bool Cluster::Centroid::isValid() const {
        return __valid;
    }

    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

    void Cluster::Centroid::compute() {
        if(__c.__size <= 0) {
            toInfinity();
        } else {
            double computeHelper = 0;
            for (int i = 0; i < __dimensions; i++) {
                computeHelper = 0;
                for (int j = 0; j < __c.__size; j++) {
                    computeHelper = computeHelper + __c[j][i];
                }
                computeHelper = computeHelper / __c.__size;
                __p[i] = computeHelper;

            }
        }
        if(__valid == false) {
            __valid = true;
        }
    }

    bool Cluster::Centroid::equal(const Point &p) const {
        for (int i = 0; i < __dimensions; i++) {
            if (__p[i] != p[i]) {
                return false;
            }
        }
        return true;
    }

    void Cluster::Centroid::toInfinity() {
        for (int i = 0; i < __dimensions; i++) {
            __p[i] = std::numeric_limits<double>::max();
        }
    }

    Cluster::Cluster(unsigned int d) : centroid(d, *this) {
        __id = ++__idGenerator;
        __size = 0;
        __points = NULL;
        __dimensionality = d;
    }

    // *********
    // BIG THREE
    // *********

    // Copy Constructor
    Cluster::Cluster(const Cluster &clust) : Cluster(clust.__dimensionality) {
        if(&clust != this) {
            __size = clust.getSize();
            if (__size != 0) {
                __points = new LNode(clust.__points->point, nullptr);

                LNodePtr cpyPtr = __points;
                LNodePtr cpyHelp = cpyPtr;
                LNodePtr nullCheck = clust.__points->next;
                while (nullCheck) {
                    cpyPtr = new LNode(nullCheck->point, nullptr);
                    cpyHelp->next = cpyPtr;
                    cpyHelp = cpyPtr;
                    nullCheck = nullCheck->next;
                }
            } else {
                __points = nullptr;
            }

            centroid.compute();
            __id = clust.__id;
        }

    }

    // Operator Overload=
    Cluster &Cluster::operator=(const Cluster &clust) {
        if(__points) {
            LNodePtr delPtr = __points;
            LNodePtr delHelp = delPtr->next;
            for (int i = 0; i < __size; i++) {
                delete delPtr;
                delPtr = delHelp;
                if (delPtr->next) {
                    delHelp = delPtr->next;
                }
            }
        }
        __points = nullptr;
        __size = 0;
        for (int i = 0; i < clust.getSize(); i++) {
            add(clust[i]);
        }

        centroid.compute();
        __id = clust.__id;
        return *this;
    }

    // Destructors
    Cluster::~Cluster() {
        if(__points != nullptr) {
            LNodePtr delPtr = __points;
            LNodePtr delHelp = nullptr;
            for (int i = 0; i < __size; i++) {
                if (__points != nullptr) {
                    delHelp = delPtr->next;
                    delete delPtr;
                    delPtr = delHelp;

                }
            }
        }
        __points = nullptr;
        __size = 0;
    }

    // ****************
    // MEMBER FUNCTIONS
    // ****************

    // Accessors and Mutators
    unsigned int Cluster::getSize() const {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const {
        return __id;
    }

    // Set functions: They allow calling c1.add(c2.remove(p));
    void Cluster::add(const Point &p) {
        if(__dimensionality == p.getDims()) {
            LNodePtr addPtr = new LNode(p, nullptr);
            if (!__points) {
                __points = addPtr;
                __size++;
                centroid.setValid(false);
                return;
            } else {
                if (contains(p)) {
                    return;
                }
                centroid.setValid(false);
                LNodePtr curr = __points;
                LNodePtr previous = nullptr;
                while(curr) {
                    if (p < curr->point) {
                        if (previous != nullptr) {
                            previous->next = new LNode(p, curr);
                            __size++;
                            return;
                        } else {
                            __points = new LNode(p, curr);
                            __size++;
                            return;
                        }

                    }
                    previous = curr;
                    curr = curr->next;
                }
                previous->next = new LNode(p, nullptr);
                __size++;
            }
        } else {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
    }

    const Point &Cluster::remove(const Point &p) {
        if(__dimensionality == p.getDims()) {
            if(!contains(p)) {
                return p;
            } else {
                centroid.setValid(false);
                LNodePtr remPtr = __points;
                LNodePtr remHelp = nullptr;
                while (remPtr) {
                    if (remPtr->point == p) {
                        if (remHelp) {
                            remHelp->next = remPtr->next;
                            delete remPtr;
                            __size--;
                            return p;
                        } else {
                            if(__size >= 2) {
                                __points = remPtr->next;
                            } else {
                                __points = nullptr;
                            }
                            delete remPtr;
                            __size--;
                            return p;
                        }
                    }
                    remHelp = remPtr;
                    remPtr = remPtr->next;
                }
                return p;
            }
        } else {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
    }

    bool Cluster::contains(const Point &p) const {
        if (__dimensionality != p.getDims()) {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        LNodePtr containCheck = __points;
        bool contain = false;
        while (containCheck) {
            if(containCheck->point == p) {
                contain = true;
            }
            containCheck = containCheck->next;
        }
        return contain;
    }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
        double dist1;
        double dist2;
        int maxDistPoint;
        bool pick;
        if (k < __size) {
            if (k <= 100) {
                *(pointArray[0]) = __points->point;
                for (int i = 1; i < k; i++) {
                    dist1 = 0;
                    maxDistPoint = 0;
                    for (int j = 0; j < __size; j++) {
                        dist2 = 0;
                        pick = false;

                        for (int l = 0; l < i; l++) {
                            dist2 = dist2 + ((*this)[j]).distanceTo(*(pointArray[l]));
                            if ((*this)[j] == *(pointArray[l])) {
                                pick = true;
                            }
                        }

                        dist2 = dist2/i;

                        if (dist2 > dist1) {
                            if (pick == false) {
                                dist1 = dist2;
                                maxDistPoint = j;
                            }
                        }
                    }
                    *(pointArray[i]) = (*this)[maxDistPoint];
                }
            } else {
                for (int i = 0; i < k; i++) {
                    *(pointArray[i]) = (*this)[i];
                }
            }
        } else {
            for (int i = 0; i < __size; i++) {
                *(pointArray[i]) = (*this)[i];
            }
            if (k != __size) {
                for(int i = __size; i < k; i++) {
                    for (int j = 0; j < __dimensionality; j++) {
                        pointArray[i]->setValue(j, std::numeric_limits<double>::max());
                    }
                }
            }
        }
    }

    // Overloaded operators

    // Members: Subscript
    const Point &Cluster::operator[](unsigned int index) const {
        if (__size > 0) {
            if (index >= __size) {
                throw OutOfBoundsEx(__size, index);
            }
            LNodePtr indexPtr = __points;
            if (index != 0) {
                for (int i = 0; i < index; i++) {
                    indexPtr = indexPtr->next;
                }
            }
            return indexPtr->point;
        } else {
            throw EmptyClusterEx();
        }
    }

    // Members: Compound assignment (Point argument)
    Cluster &Cluster::operator+=(const Point &p) {

            add(p);

        return *this;
    }

    Cluster &Cluster::operator-=(const Point &p) {
        if (this->contains(p)) {
            remove(p);
        }
        return *this;
    }

    // Members: Compound assignment (Cluster argument)
    Cluster &Cluster::operator+=(const Cluster &c) {
        // union
        if (__dimensionality == c.getDimensionality()) {
            LNodePtr cPtr = c.__points;
            while (cPtr) {
                if (this->contains(cPtr->point)) {
                    cPtr = cPtr->next;
                } else {
                    add(cPtr->point);
                    cPtr = cPtr->next;
                }
            }
            return *this;
        } else {
            throw DimensionalityMismatchEx(__dimensionality, c.getDimensionality());
        }
    }

    Cluster &Cluster::operator-=(const Cluster &c) {
        // (asymmetric) difference
        if (__dimensionality == c.getDimensionality()) {
            LNodePtr cPtr = c.__points;
            while (cPtr) {
                if (this->contains(cPtr->point)) {
                    remove(cPtr->point);
                }
                cPtr = cPtr->next;
            }
            return *this;
        } else {
            throw DimensionalityMismatchEx(__dimensionality, c.getDimensionality());
        }
    }

    // *******
    // FRIENDS
    // *******

    // Friends: IO
    std::ostream &operator<<(std::ostream &out, const Cluster &c) {
        out << std::setprecision(20);
        for (int i = 0; i < c.getSize(); i++) {
            out << c[i] << " " << POINT_CLUSTER_ID_DELIM << " " << c.__id << std::endl;
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, Cluster &c) {
        std::string string1;
        while (getline(in,string1)) {
            int dim = (int) std::count(string1.begin(), string1.end(), Point::POINT_VALUE_DELIM);
            Point newPoint(++dim);
            std::stringstream inputStringStream(string1);
            try {
                inputStringStream >> newPoint;
                c.add(newPoint);
                c.add(newPoint);
            }
            catch (DimensionalityMismatchEx& e) {
                std::cerr << "Caught an exception of type: " << e << std::endl;
                newPoint.rewindIdGen();
            }
            catch (...) {
                std::cerr << "Caught an unknown exception" << std::endl;
                newPoint.rewindIdGen();
            }
        }
        return in;
    }

    // Friends: Comparison
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.getDimensionality() == rhs.getDimensionality()) {
            if (lhs.getSize() == rhs.getSize()) {
                LNodePtr lhsPtr = lhs.__points;
                LNodePtr rhsPtr = rhs.__points;
                while (lhsPtr && rhsPtr) {
                    if (lhsPtr->point != rhsPtr->point) {
                        return false;
                    }
                    lhsPtr = lhsPtr->next;
                    rhsPtr = rhsPtr->next;
                }
                return true;
            }
            return false;
        } else {
            throw DimensionalityMismatchEx(lhs.getDimensionality(), rhs.getDimensionality());
        }
    }

    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        return !(lhs == rhs);
    }

    // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &lhs, const Point &rhs) {
        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }

    const Cluster operator-(const Cluster &lhs, const Point &rhs) {
        Cluster difference(lhs);
        difference -= rhs;
        return difference;
    }

    // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {
        // union
        if (lhs.getDimensionality() == rhs.getDimensionality()) {
            Cluster sum(lhs);
            sum += rhs;
            return sum;
        } else {
            throw DimensionalityMismatchEx(lhs.getDimensionality(), rhs.getDimensionality());
        }
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {
        // (asymmetric) difference
        if (lhs.getDimensionality() == rhs.getDimensionality()) {
            Cluster difference(lhs);
            difference -= rhs;
            return difference;
        } else {
            throw DimensionalityMismatchEx(lhs.getDimensionality(), rhs.getDimensionality());
        }
    }

    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to) { }

    void Cluster::Move::perform() {
        if (__from.contains(__p)) {
            __to.add(__from.remove(__p));
        }
        if (__from.__size == 0) {
            __from.centroid.toInfinity();
        }
        if(__to.__size == 0) {
            __to.centroid.toInfinity();
        }
    }


}

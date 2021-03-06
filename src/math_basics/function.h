//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_FUNCTION_H
#define QTKNOTS_FUNCTION_H

#include "matrix.h"
#include <vector>
#include "../log.h"

class function {
public:
    std::string NameOfFunction;
    complex centerX;
    complex centerY;

    function(std::string name) : NameOfFunction(name), centerX(0.0, 0.0), centerY(0.0, 0.0) {}

    virtual complex x(complex z) = 0;

    virtual complex y(complex z) = 0;

    virtual bool computePoints(std::vector<fourvector> &resultPoints,
                               double radius, long double h,
                               double steps_multiplier) = 0;

    virtual ~function() = 0;

    fourvector centerPoint() {
      return fourvector(centerX.real(), centerX.imag(), centerY.real(), centerY.imag());
    }

    void checkIfHole(long int &stashcount, long int &stashcount2, short &isEnd);
    void checkIfDistanceMakesSence(long double &odsuw3, long double &refdlug,
                                   fourvector &abyraz, short &isEnd,
                                   fourvector &refpunkt2);

    int seed = time(NULL);
    double RandomValue();
};


#endif //QTKNOTS_FUNCTION_H

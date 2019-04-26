//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_FUNCTION_H
#define QTKNOTS_FUNCTION_H

#include "matrix.h"
#include <QString>
#include <vector>

class function {
public:
    QString nameofmap;
    static complex centerX;
    static complex centerY;

    function(QString n) : nameofmap(n) {}

    virtual complex x(complex z) = 0;

    virtual complex y(complex z) = 0;

    virtual bool computePoints(std::vector<fourvector> &resultPoints,
                               double radius, long double h,
                               double steps_multiplier) = 0;

    static fourvector centerPoint() {
      return fourvector(centerX.real(), centerX.imag(), centerY.real(), centerY.imag());
    }
};


#endif //QTKNOTS_FUNCTION_H

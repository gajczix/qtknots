//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_PARAMETR_H
#define QTKNOTS_PARAMETR_H

#include "function.h"

/**
 * function from C into C, part of parametrisation.
 */
typedef complex (*param)(complex);

complex empty_function(complex);

class parametr : public function {
private:
    param Px;
    param Py;
    param Dx;
    param Dy;

public:
    parametr(param X, param Y, param DX, param DY, std::string n) : function(n) {
      Px = X;
      Py = Y;
      Dx = DX;
      Dy = DY;
    }

    parametr() : function("empty fun") {
      Px = empty_function;
      Py = empty_function;
      Dx = empty_function;
      Dy = empty_function;
    }

    /// reverse complex point to get it in 4 dimensions.
    fourvector pointOn4Sphere(complex z) {
      return fourvector(Px(z).real(), Px(z).imag(), Py(z).real(), Py(z).imag());
    }

    complex x(complex z) override { return Px(z); }

    complex y(complex z) override { return Py(z); }

    /// Returns derivative of function  |x-centerX|^2+|y-centerY|^2 over dz.
    complex dz(complex z) {
      return Dx(z) * (std::conj(Px(z)) - std::conj(centerX)) +
             Dy(z) * (std::conj(Py(z)) - std::conj(centerY));
    }

    // ortogonal to derivative of radius.
    complex pdz(complex z) {
      complex a = dz(z);
      if (a != complex(0, 0))
        a = a * (1.0 / std::abs(a));
      return complex(a.imag(), a.real());
    }

    complex ndz(complex z) {
      complex a = dz(z);
      if (!(a == 0.0))
        a = a * (1.0 / std::abs(a));
      return std::conj(a);
    }

    long double distance(complex z) {
      return (pointOn4Sphere(z) - centerPoint()).length();
    }

    bool getToThePoint(long double h, long double r, complex &zstart);

    bool computePoints(std::vector<fourvector> &resultPoints, double radius,
                       long double h, double steps_multiplier) override;
};




#endif //QTKNOTS_PARAMETR_H

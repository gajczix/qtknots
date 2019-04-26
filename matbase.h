#ifndef MATBASE_H
#define MATBASE_H

#include "log.h"
#include "fourvector.h"
#include <QString>
#include <complex>
#include <cstdlib>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_math.h>
#include <string>
#include <vector>

#define _SIZE 10

using complex = std::complex<double>;

class fourvector;

complex operator*(complex comp, float scalar);

complex operator*(float scalar, complex comp);

class matrix {
private:
    const static int _size = _SIZE;

public:
    int size() { return _size; }

    complex coef[_size][_size];

    matrix giveDx();

    matrix giveDy();

    complex compute(complex x, complex y);

    complex compute(fourvector v);
};

/**
 * function from C into C, part of parametrisation.
 */
typedef complex (*param)(complex);

complex empty_function(complex);

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

/**
 * Class to represent all maps from C^2 to C.
 */
class map : public function {
private:
    /// Function to analyze, from C^2, given as matrix of polynomials
    matrix fun_matrix;
    std::string secret_name = "";

public:
    map(matrix g, QString n) : function(n), fun_matrix(g) {}

    map(complex g[_SIZE][_SIZE], QString n) : function(n) {
      for (int i = 0; i < fun_matrix.size(); i++) {
        for (int j = 0; j < fun_matrix.size(); j++) {
          fun_matrix.coef[i][j] = g[i][j];
        }
      }
    }

    map(std::vector<std::pair<std::pair<int, int>, complex>> m, QString n)
        : function(n) {
      for (int i = 0; i < _SIZE; i++) {
        for (int j = 0; j < _SIZE; j++) {
          fun_matrix.coef[i][j] = 0;
        }
      }

      for (auto &elem : m) {
        int i = elem.first.first;
        int j = elem.first.second;
        complex value = elem.second;

        fun_matrix.coef[i][j] = complex(value.real(), value.imag());
      }
    }

    std::string get_name() { return secret_name; }

    map();

    complex x(__attribute__((unused)) complex z) override { return complex(); }

    complex y(__attribute__((unused)) complex z) override { return complex(); }

    bool getToThePoint(long double r, fourvector &zstart);

    bool computePoints(std::vector<fourvector> &resultPoints, double radius,
                       long double h, double steps_multiplier) override;

    complex w(complex z1, complex z2) { return fun_matrix.compute(z1, z2); }

    complex w(fourvector T) {
      return fun_matrix.compute(complex(T.x(), T.y()), complex(T.z(), T.u()));
    }

    complex w1(fourvector T) {
      return fun_matrix.giveDx().compute(complex(T.x(), T.y()),
                                         complex(T.z(), T.u()));
    }

    complex w2(fourvector T) {
      return fun_matrix.giveDy().compute(complex(T.x(), T.y()),
                                         complex(T.z(), T.u()));
    }

    fourvector trajectory(fourvector K);

    static complex value;
    static long double deviation;

    void printCoefMatrix();

    void printMapMatrix();
};

class parametr : public function {
private:
    param Px;
    param Py;
    param Dx;
    param Dy;

public:
    parametr(param X, param Y, param DX, param DY, QString n) : function(n) {
      Px = X;
      Py = Y;
      Dx = DX;
      Dy = DY;
    }

    parametr() : function(QString("empty fun")) {
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


#endif

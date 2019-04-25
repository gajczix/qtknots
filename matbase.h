#ifndef MATBASE_H
#define MATBASE_H
#include "log.h"
#include <QString>
#include <complex>
#include <cstdlib>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_math.h>
#include <string>
#include <vector>

#define _SIZE 10

class fourvector;

std::complex<double> operator*(std::complex<double> comp, float scalar);
std::complex<double> operator*(float scalar, std::complex<double> comp);

class matrix {
private:
  const static int _size = _SIZE;

public:
  int size() { return _size; }
  std::complex<double> coef[_size][_size];
  matrix giveDx();
  matrix giveDy();
  std::complex<double> compute(std::complex<double> x, std::complex<double> y);
  std::complex<double> compute(fourvector v);
};

/**
 * function from C^2 into C.
 */
typedef std::complex<double> (*fun)(std::complex<double>, std::complex<double>);
/**
 * function from C into C, part of parametrisation.
 */
typedef std::complex<double> (*param)(std::complex<double>);
std::complex<double> empty_fun(std::complex<double>, std::complex<double>);
std::complex<double> empty_fun2(std::complex<double>);

class fourvector {
private:
  long double wx, wy, wz, wu;

public:
  long double x() { return wx; }
  long double y() { return wy; }
  long double z() { return wz; }
  long double u() { return wu; }
  fourvector(long double a, long double b, long double c, long double d) {
    wx = a;
    wy = b;
    wz = c;
    wu = d;
  }
  fourvector() {
    wx = 0;
    wy = 0;
    wz = 0;
    wu = 0;
  }
  fourvector operator+(fourvector T) {
    return fourvector(wx + T.wx, wy + T.wy, wz + T.wz, wu + T.wu);
  }
  fourvector operator-(fourvector T) {
    return fourvector(wx - T.wx, wy - T.wy, wz - T.wz, wu - T.wu);
  }
  fourvector operator*(long double d) {
    return fourvector(wx * d, wy * d, wz * d, wu * d);
  }
  bool operator==(fourvector T) {
    return ((wx == T.wx) && (wy == T.wy) && (wz == T.wz) && (wu == T.wu));
  }
  long double squareLenght() { return wx * wx + wy * wy + wz * wz + wu * wu; }
  long double length();

  std::string to_string() {
    char buffer[100];
    sprintf(buffer, "(%Lf, %Lf, %Lf, %Lf)", this->x(), this->y(), this->z(),
            this->u());
    return buffer;
  }
  //  long double operator*(fourvector T);
};

class function {
public:
  QString nameofmap;
  static std::complex<double> centerX;
  static std::complex<double> centerY;
  function(QString n) : nameofmap(n) {}

  virtual std::complex<double> x(std::complex<double> z) = 0;

  virtual std::complex<double> y(std::complex<double> z) = 0;

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

  map(std::complex<double> g[_SIZE][_SIZE], QString n) : function(n) {
    for (int i = 0; i < fun_matrix.size(); i++) {
      for (int j = 0; j < fun_matrix.size(); j++) {
        fun_matrix.coef[i][j] = g[i][j];
      }
    }
  }

  map(std::vector<std::pair<std::pair<int, int>, std::complex<double>>> m, QString n)
      : function(n) {
    for (int i = 0; i < _SIZE; i++) {
      for (int j = 0; j < _SIZE; j++) {
        fun_matrix.coef[i][j] = 0;
      }
    }

    for (auto &elem : m) {
      int i = elem.first.first;
      int j = elem.first.second;
      std::complex<double> value = elem.second;

      fun_matrix.coef[i][j] = std::complex<double>(value.real(), value.imag());
    }
  }

  std::string get_name() { return secret_name; }

  map();

  std::complex<double> x(__attribute__((unused)) std::complex<double> z) override { return std::complex<double>(); }
  std::complex<double> y(__attribute__((unused)) std::complex<double> z) override { return std::complex<double>(); }

  bool getToThePoint(long double r, fourvector &zstart);
  bool computePoints(std::vector<fourvector> &resultPoints, double radius,
                     long double h, double steps_multiplier) override;

  std::complex<double> w(std::complex<double> z1, std::complex<double> z2) { return fun_matrix.compute(z1, z2); }
  std::complex<double> w(fourvector T) {
    return fun_matrix.compute(std::complex<double>(T.x(), T.y()), std::complex<double>(T.z(), T.u()));
  }
  std::complex<double> w1(fourvector T) {
    return fun_matrix.giveDx().compute(std::complex<double>(T.x(), T.y()),
                                       std::complex<double>(T.z(), T.u()));
  }
  std::complex<double> w2(fourvector T) {
    return fun_matrix.giveDy().compute(std::complex<double>(T.x(), T.y()),
                                       std::complex<double>(T.z(), T.u()));
  }
  fourvector trajectory(fourvector K);
  static std::complex<double> value;
  static long double deviation;

  void printCoefMatrix();

  void printFriendlyCoefMatrix();
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
    Px = empty_fun2;
    Py = empty_fun2;
    Dx = empty_fun2;
    Dy = empty_fun2;
  }
  /// reverse complex point to get it in 4 dimensions.
  fourvector pointOn4Sphere(std::complex<double> z) {
    return fourvector(Px(z).real(), Px(z).imag(), Py(z).real(), Py(z).imag());
  }
  std::complex<double> x(std::complex<double> z) override { return Px(z); }
  std::complex<double> y(std::complex<double> z) override { return Py(z); }
  /// Returns derivative of function  |x-centerX|^2+|y-centerY|^2 over dz.
  std::complex<double> dz(std::complex<double> z) {
    return Dx(z) * (std::conj(Px(z)) - std::conj(centerX)) +
           Dy(z) * (std::conj(Py(z)) - std::conj(centerY));
  }

  // ortogonal to derivative of radius.
  std::complex<double> pdz(std::complex<double> z) {
    std::complex<double> a = dz(z);
    if (a != std::complex<double>(0,0))
      a = a * (1.0 / std::abs(a));
    return std::complex<double>(a.imag(), a.real());
  }

  std::complex<double> ndz(std::complex<double> z) {
    std::complex<double> a = dz(z);
    if (!(a == 0.0))
      a = a * (1.0 / std::abs(a));
    return std::conj(a);
  }
  long double distance(std::complex<double> z) {
    return (pointOn4Sphere(z) - centerPoint()).length();
  }

  bool getToThePoint(long double h, long double r, std::complex<double> &zstart);
  bool computePoints(std::vector<fourvector> &resultPoints, double radius,
                     long double h, double steps_multiplier) override;
};

fourvector operator*(long double d, fourvector T);
fourvector ortogonal(fourvector A, fourvector B, fourvector C);
long double operator*(fourvector A, fourvector B);

#endif

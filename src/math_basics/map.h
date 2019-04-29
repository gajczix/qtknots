//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_MAP_H
#define QTKNOTS_MAP_H

#include "function.h"

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



#endif //QTKNOTS_MAP_H

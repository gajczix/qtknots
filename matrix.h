//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_MATRIX_H
#define QTKNOTS_MATRIX_H

#include "fourvector.h"
#include <complex>

#define _SIZE 10

using complex = std::complex<double>;

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


#endif //QTKNOTS_MATRIX_H

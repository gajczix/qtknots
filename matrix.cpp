//
// Created by Krystyna Gajczyk on 26/04/19.
//

#include "matrix.h"



matrix matrix::giveDx() {
  matrix Dx;
  int n = this->size();
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < n; j++) {
      Dx.coef[i - 1][j] = this->coef[i][j] * i;
    }
  }
  return Dx;
}

matrix matrix::giveDy() {
  matrix Dy;
  int n = this->size();
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      Dy.coef[i][j - 1] = this->coef[i][j] * j;
    }
  }
  return Dy;
}

complex pow(complex x, int n) {
  complex result = 1;
  for (int i = 0; i < n; i++) {
    result = x * result;
  }
  return result;
}

complex matrix::compute(fourvector v) {
  complex x(v.x(), v.y());
  complex y(v.z(), v.u());
  return compute(x, y);
}

complex matrix::compute(complex x, complex y) {
  complex result = 0;
  int n = this->size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      result = result + pow(x, i) * pow(y, j) * this->coef[i][j];
    }
  }
  return result;
}

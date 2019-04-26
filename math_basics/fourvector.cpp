//
// Created by Krystyna Gajczyk on 26/04/19.
//
#include "fourvector.h"

long double fourvector::operator*(fourvector A) {
  fourvector B = *this;
  return A.x() * B.x() + A.y() * B.y() + A.z() * B.z() + A.u() * B.u();
}

fourvector operator*(long double scalar, fourvector vector){
  return vector * scalar;
}

fourvector ortogonal(fourvector A, fourvector B, fourvector C) {

  A = A * (1.0 / A.length());
  B = B * (1.0 / B.length());
  C = C * (1.0 / C.length());

  long double K1 = A.y() * B.z() * C.u() + A.z() * B.u() * C.y() +
                   A.u() * B.y() * C.z() - A.u() * B.z() * C.y() -
                   B.u() * C.z() * A.y() - C.u() * A.z() * B.y();
  long double K2 = A.x() * B.z() * C.u() + A.z() * B.u() * C.x() +
                   A.u() * B.x() * C.z() - A.u() * B.z() * C.x() -
                   B.u() * C.z() * A.x() - C.u() * A.z() * B.x();
  long double K3 = A.x() * B.y() * C.u() + A.y() * B.u() * C.x() +
                   A.u() * B.x() * C.y() - A.u() * B.y() * C.x() -
                   B.u() * C.y() * A.x() - C.u() * A.y() * B.x();
  long double K4 = A.x() * B.y() * C.z() + A.y() * B.z() * C.x() +
                   A.z() * B.x() * C.y() - A.z() * B.y() * C.x() -
                   B.z() * C.y() * A.x() - C.z() * A.y() * B.x();
  K1 = -K1;
  K3 = -K3;
  fourvector E(K1, K2, K3, K4);
  // we normalize vector.
  return E * (1.0 / E.length());
}

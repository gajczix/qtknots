//
// Created by Krystyna Gajczyk on 26/04/19.
//

#ifndef QTKNOTS_FOURVECTOR_H
#define QTKNOTS_FOURVECTOR_H

#include <string>

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

    long double operator*(fourvector T);


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
};

fourvector operator*(long double scalar, fourvector vector);

fourvector ortogonal(fourvector A, fourvector B, fourvector C);

#endif //QTKNOTS_FOURVECTOR_H

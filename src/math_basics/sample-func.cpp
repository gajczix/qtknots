#include <complex>

#include "sample-func.h"

complex swallowtail_x(complex z) { return z * z * z - z * 3.0; }
complex swallowtail_y(complex z) { return z * z * z * z - z * z * 2.0; }
complex swallowtail_dx(complex z) { return z * z * 3.0 - 3.0; }
complex swallowtail_dy(complex z) { return z * z * z * 4.0 - z * 4.0; }

complex t23_x(complex z) { return z * z; }
complex t23_y(complex z) { return z * z * z; }
complex t23_dx(complex z) { return z * 2.0; }
complex t23_dy(complex z) { return z * z * 3.0; }

complex t34_x(complex z) { return z * z * z; }
complex t34_y(complex z) { return z * z * z * z; }
complex t34_dx(complex z) { return z * z * 3.0; }
complex t34_dy(complex z) { return z * z * z * 4.0; }

complex t35_x(complex z) { return z * z * z; }
complex t35_y(complex z) { return z * z * z * z * z; }
complex t35_dx(complex z) { return z * z * 3.0; }
complex t35_dy(complex z) { return z * z * z * z * 5.0; }

complex t25_x(complex z) { return z * z; }
complex t25_y(complex z) { return z * z * z * z * z; }
complex t25_dx(complex z) { return z * 2.0; }
complex t25_dy(complex z) { return z * z * z * z * 5.0; }

complex petlax(complex z) { return z * z + complex(1, 0); }
complex petlay(complex z) { return z * z * z + z; }
complex petladx(complex z) { return z * 2.0; }
complex petlady(complex z) { return z * z * 3.0 + 1.0; }

complex T234_x(complex t) { return t * t + t * t * t; }
complex T234_y(complex t) { return t * t * t * t; }
complex T234_dx(complex t) { return 2 * t + 3 * t * t; }
complex T234_dy(complex t) { return 4 * t * t * t; }

complex T235_x(complex t) { return t * t; }
complex T235_y(complex t) { return t * t * t + t * t * t * t * t; }
complex T235_dx(complex t) { return 2 * t; }
complex T235_dy(complex t) { return 3 * t * t + 5 * t * t * t * t; }

parametr swallowtail_p(swallowtail_x, swallowtail_y, swallowtail_dx, swallowtail_dy,
               "Swallowtail");
map swallowtail({{{4, 0}, 1},
          {{2, 1}, 6},
          {{0, 3}, -1},
          {{2, 0}, -2},
          {{0, 2}, 6},
          {{0, 1}, -9}},
         "map:Swallowtail");

parametr Petlap(petlax, petlay, petladx, petlady, "y^2=x^3-x^2");
map Petla({{{3, 0}, 1}, {{2, 0}, -1}, {{0, 2}, -1}},
          "map:y^2=x^3-x^2");

parametr T23p(t23_x, t23_y, t23_dx, t23_dy, "x^2-y^3");
map T23({{{2, 0}, 1}, {{0, 3}, -1}}, "map:x^2-y^3");

parametr T25p(t25_x, t25_y, t25_dx, t25_dy, "x^2-y^5");

map T25({{{2, 0}, 1}, {{0, 5}, -1}}, "map:x^2-y^5");

parametr T34p(t34_x, t34_y, t34_dx, t34_dy, "x^3-y^4");
map T34({{{3, 0}, 1}, {{0, 4}, -1}}, "map:x^3-y^4");

parametr T35p(t35_x, t35_y, t35_dx, t35_dy, "x^3-y^5");
map T35({{{3, 0}, 1}, {{0, 5}, -1}}, "map:x^3-y^5");

parametr T234p(T234_x, T234_y, T234_dx, T234_dy, "T234");
map T234({{{4, 0}, 1}, {{2, 1}, -2}, {{1, 2}, -4}, {{0, 3}, -1}, {{0, 2}, 1}},
         "map:T234");

parametr T235p(T235_x, T235_y, T235_dx, T235_dy, "x^3-y^5");

map RandomFunc;

using namespace std::complex_literals;

void initialize_maps(std::vector<function *> &pv) {
  std::vector<function *> ParametrVector;

  ParametrVector.push_back(&RandomFunc);
  write_log(RandomFunc.get_name());
  ParametrVector.push_back(&swallowtail_p);
  ParametrVector.push_back(&Petlap);
  ParametrVector.push_back(&T23p);
  ParametrVector.push_back(&T25p);
  ParametrVector.push_back(&T34p);
  ParametrVector.push_back(&T35p);
  ParametrVector.push_back(&T234p);
  ParametrVector.push_back(&T235p);
  ParametrVector.push_back(&swallowtail);
  ParametrVector.push_back(&Petla);
  ParametrVector.push_back(&T23);
  ParametrVector.push_back(&T25);
  ParametrVector.push_back(&T34);
  ParametrVector.push_back(&T35);
  ParametrVector.push_back(&T234);

  RandomFunc.printMapMatrix();
  pv = ParametrVector;
}

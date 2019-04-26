#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>

#include "matbase.h"

/// Definitions of multiple functions

complex swallowtail_x(complex z);
complex swallowtail_y(complex z);
complex swallowtail_dx(complex z);
complex swallowtail_dy(complex z);

complex t23_x(complex z);
complex t23_y(complex z);
complex t23_dx(complex z);
complex t23_dy(complex z);

complex t34_x(complex z);
complex t34_y(complex z);
complex t34_dx(complex z);
complex t34_dy(complex z);

complex t35_x(complex z);
complex t35_y(complex z);
complex t35_dx(complex z);
complex t35_dy(complex z);

complex t25_x(complex z);
complex t25_y(complex z);
complex t25_dx(complex z);
complex t25_dy(complex z);

complex petlax(complex z);
complex petlay(complex z);
complex petladx(complex z);
complex petlady(complex z);

extern parametr swallowtail_p;

void initialize_maps(std::vector<function *> &pv);

#endif

#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <vector>

#include "matbase.h"

/// Definitions of multiple functions

std::complex<double> jaskolkax(std::complex<double> z);
std::complex<double> jaskolkay(std::complex<double> z);
std::complex<double> jaskolkadx(std::complex<double> z);
std::complex<double> jaskolkady(std::complex<double> z);

std::complex<double> t23_x(std::complex<double> z);
std::complex<double> t23_y(std::complex<double> z);
std::complex<double> t23_dx(std::complex<double> z);
std::complex<double> t23_dy(std::complex<double> z);

std::complex<double> t34_x(std::complex<double> z);
std::complex<double> t34_y(std::complex<double> z);
std::complex<double> t34_dx(std::complex<double> z);
std::complex<double> t34_dy(std::complex<double> z);

std::complex<double> t35_x(std::complex<double> z);
std::complex<double> t35_y(std::complex<double> z);
std::complex<double> t35_dx(std::complex<double> z);
std::complex<double> t35_dy(std::complex<double> z);

std::complex<double> t25_x(std::complex<double> z);
std::complex<double> t25_y(std::complex<double> z);
std::complex<double> t25_dx(std::complex<double> z);
std::complex<double> t25_dy(std::complex<double> z);

std::complex<double> petlax(std::complex<double> z);
std::complex<double> petlay(std::complex<double> z);
std::complex<double> petladx(std::complex<double> z);
std::complex<double> petlady(std::complex<double> z);

extern parametr jaskp;

void initialize_maps(std::vector<function *> &pv);

#endif

//
// Created by Krystyna Gajczyk on 26/04/19.
//

#include "function.h"

void function::checkIfHole(long int &stashcount, long int &stashcount2, short &isEnd) {
  if (stashcount < (stashcount2 + 2)) {
    write_log("We are in the hole.\n");
    isEnd = 4;
  }
  stashcount2 = stashcount;
}

function::~function() {};


void function::checkIfDistanceMakesSence(long double &odsuw3, long double &refdlug,
                               fourvector &abyraz, short &isEnd,
                               fourvector &refpunkt2) {
  odsuw3 = (abyraz - refpunkt2).length() - refdlug;
  if (odsuw3 < 0) {
    odsuw3 = -odsuw3;
  }
  odsuw3 = odsuw3 / (1 + refdlug);
  if (odsuw3 > 0.01) {
    char buffer[1000];
    sprintf(buffer,
            "Change too big! Current distance %Lf, distance after change %Lf, "
                "distance %Lf.\n",
            (abyraz - refpunkt2).length(), refdlug, odsuw3);
    write_log(buffer);
    isEnd = 3;
  }
}

double function::RandomValue() {
  if (seed) {
    std::srand(time(NULL));
    seed = false;
  }
  auto rand = (std::rand() % 2000000000) * 0.00000001 - 10.0;
  return rand;
}
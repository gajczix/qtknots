//
// Created by Krystyna Gajczyk on 26/04/19.
//
#include "parametr.h"

// Newton method
complex fakeDoStep(parametr m, complex StartPoint, long double skok) {
  complex z0 = m.pdz(StartPoint);
  complex z1 = StartPoint + z0 * skok;
  return StartPoint + (z0 + m.pdz(z1)) * (0.5 * skok);
}

// Function to search on complex plane intersection with sphere.
bool parametr::getToThePoint(long double h, long double r, complex &zstart) {
  complex z = zstart;
  complex z1, z2, z3, z4;
  long double k1, k2, k3, k4;
  static complex kroka(1.0, 0.0);
  static complex krokb(-1.0, 0.0);
  static complex krokc(0.0, 1.0);
  static complex krokd(0.0, -1.0);

  long double skok = h * 100.0;
  long double k = std::abs(this->distance(z) - r);
  long n = 0;
  while ((skok != 0.0) && (k > h) && (n <= 100000)) {
    n++;
    z1 = z + kroka * skok;
    z2 = z + krokb * skok;
    z3 = z + krokc * skok;
    z4 = z + krokd * skok;

    k1 = std::abs(this->distance(z1) - r);
    k2 = std::abs(this->distance(z2) - r);
    k3 = std::abs(this->distance(z3) - r);
    k4 = std::abs(this->distance(z4) - r);
    if ((k <= k1) && (k <= k2) && (k <= k3) && (k <= k4)) {
      skok *= 0.5;
    } else {
      if ((k1 <= k2) && (k1 <= k3) && (k1 <= k4)) {
        z = z1;
      } else if ((k2 <= k3) && (k2 <= k4)) {
        z = z2;
      } else if (k3 <= k4) {
        z = z3;
      } else {
        z = z4;
      }

      k = std::abs(this->distance(z) - r);
    }
  }
  if (k <= h) {
    zstart = z;
    return true;
  }
  printf("-");
  return false;
}

bool parametr::computePoints(std::vector<fourvector> &resultPoints,
                             double radius, long double h,
                             double steps_multiplier) {
  complex Zstart;
  int count = 0;
  do {
    count++;
    Zstart = complex(RandomValue(), RandomValue());
  } while (!this->getToThePoint(0.001, radius, Zstart) && (count < 1000));
  if (count > 999) {
    write_log("It looks like the intersection is empty \n");
    return false;
  }

  std::vector<fourvector> points;

  long int step_counter = 0;

  long double distance_after_100 = 0.0;
  long double distance_from_100 = 100000.0;
  long double odsuw3 = 0.0;

  complex current_point = Zstart;

  fourvector refpunkt = this->pointOn4Sphere(Zstart);
  fourvector refpunkt2 = this->centerPoint();

  points.push_back(refpunkt);

  long double refdlug = (refpunkt - refpunkt2).length();
  long double refdlug2 = refdlug * 0.01;
  long double refdlug3 = refdlug2 * refdlug2;

  short isEnd = 0;
  fourvector abyraz;
  long int stashcount = 0;
  long int stashcount2 = 0;

  do {
    step_counter++;
    current_point = fakeDoStep(*this, current_point, h);
    abyraz = this->pointOn4Sphere(current_point);

    if (step_counter == 100) {
      distance_after_100 = (abyraz - refpunkt).squareLenght();
    }

    if (step_counter > 100) {
      distance_from_100 = (abyraz - refpunkt).squareLenght();
      if (distance_from_100 <
          distance_after_100 * 0.0001) { // jesteśmy blisko, kończymy obliczenia
        isEnd = 1;
        continue;
      }
    }

    if (step_counter ==
        10000 *
        steps_multiplier) { // wykonaliśmy dużo kroków, kończymy obliczenia
      isEnd = 2;
      continue;
    }

    if (step_counter % 10 == 0) {
      if ((abyraz - points.back()).squareLenght() > refdlug3) {
        points.push_back(abyraz);
        stashcount++;
      }
    }

    if (step_counter % 10000 == 0) {
      this->checkIfHole(stashcount, stashcount2, isEnd);
    }
    if (step_counter % 1000 == 0) {
      this->checkIfDistanceMakesSence(odsuw3, refdlug, abyraz, isEnd, refpunkt2);
    }
  } while (isEnd == 0);
  points.push_back(refpunkt); // zamykamy wykres
  resultPoints = points;

  if (isEnd == 1) {
    return true;
  } else {
    return false;
  }
}

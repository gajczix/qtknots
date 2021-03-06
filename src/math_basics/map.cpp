//
// Created by Krystyna Gajczyk on 26/04/19.
//

#include "map.h"
#include "../log.h"

double randDouble() { return ((double)std::rand() / (double)RAND_MAX); }


fourvector pickRandom4v(double R) {
  double r1 = randDouble() * M_PI;     // [0, pi]
  double r2 = randDouble() * M_PI;     // [0, pi]
  double r3 = randDouble() * M_PI * 2; // [0, 2pi]

  return fourvector(R * sin(r1), R * cos(r1) * sin(r2),
                    R * cos(r1) * cos(r2) * sin(r3),
                    R * cos(r1) * cos(r2) * cos(r3));
}

void restartSearch(int &counter, int &ecounter, fourvector &z, double &scalar,
                   double r) {
  z = pickRandom4v(r);
  counter = 1;
  ecounter = 1;
  scalar = r;
}


fourvector fakeDoStep(map m, fourvector StartPoint, long double skok,
                      long double R) {
  skok *= 2;
  fourvector z0 = m.trajectory(StartPoint);
  fourvector z1 = StartPoint + z0 * skok;
  fourvector result = StartPoint + (z0 + m.trajectory(z1)) * (0.5 * skok);

  return result * (R / result.length());
}


map::map() : function("map:random") {
  std::string name = "";
  std::srand(time(NULL));
  int max_x = std::max(10, rand() % 10 + 2);
  int max_y = std::max(10, rand() % 10 + 2);
  for (int i = 0; i < max_x; i++) {
    for (int j = 0; j < max_y; j++) {
      if (i != 0 || j != 0) {
        name += " ,";
      }
      complex rand(RandomValue(), RandomValue());
      fun_matrix.coef[i][j] = rand;
      char buffer[200];
      sprintf(buffer, "%d %d %lf %lf\n", i, j, rand.real(), rand.imag());
      name += buffer;
    }
  }
  secret_name = name;
}


fourvector map::trajectory(fourvector K) {
  complex z1(K.x(), K.y());
  complex z2(K.z(), K.u());

  complex poch1 = fun_matrix.giveDx().compute(z1, z2);
  complex poch2 = fun_matrix.giveDy().compute(z1, z2);

  fourvector direction1(poch1.real(), -poch1.imag(), poch2.real(), -poch2.imag());
  fourvector direction2(poch1.imag(), poch1.real(), poch2.imag(), poch2.real());
  fourvector Apar =
      ortogonal(K, direction1, direction2); // good direction to go

  return Apar;
}


bool map::getToThePoint(long double r, fourvector &zstart) {
  fourvector z;
  double scalar;
  int counter, extra_counter;
  long long all_steps = 0;
  restartSearch(counter, extra_counter, z, scalar, r);
  complex value = fun_matrix.compute(z);

  while (std::abs(value) > 0.01) {
    counter++;
    all_steps++;
    complex poch1 = fun_matrix.giveDx().compute(z);
    complex poch2 = fun_matrix.giveDy().compute(z);

    value = fun_matrix.compute(z);

    fourvector gradient(poch1.real(), -1 * poch1.imag(), poch2.real(),
                        -1 * poch2.imag());
    gradient = gradient - ((gradient * z) * (1 / r) * z);
    extra_counter++;

    if (extra_counter > 10) {
      restartSearch(counter, extra_counter, z, scalar, r);
    }

    if (gradient.squareLenght() < 0.01 && std::abs(value) > 0.01) {
      write_log("to small gradient");
      restartSearch(counter, extra_counter, z, scalar, r);
    }

    fourvector zz = z - (gradient * scalar);

    zz = zz * (r / zz.length());

    complex valueZZ = fun_matrix.compute(zz);

    if (std::abs(value) <= std::abs(valueZZ)) {
      scalar *= 0.1;
      if (scalar == 0) {
        restartSearch(counter, extra_counter, z, scalar, r);
      }
      continue;
    }
    z = zz;
    scalar = 0.01;
    extra_counter = 1;

    if (counter > 10000 || extra_counter > 10) {
      restartSearch(counter, extra_counter, z, scalar, r);
    }
    if (z.x() == NAN) {
      restartSearch(counter, extra_counter, z, scalar, r);
    }

    if (all_steps > 4000000000) {
      write_log("too many steps, cannot find start point");
      return false;
    }
  }
  zstart = z;

  return true;
}

bool map::computePoints(std::vector<fourvector> &resultPoints, double radius,
                        long double h, double steps_multiplier) {
  fourvector startPoint;

  getToThePoint(radius, startPoint);

  std::vector<fourvector> points;

  long int step_counter = 0;

  long double distance_after_100 = 0.0;
  long double distance_from_100 = 100000.0;
  long double odsuw3 = 0.0;

  fourvector current_point = startPoint;
  fourvector refpunkt = startPoint;
  fourvector refpunkt2 = this->centerPoint();

  points.push_back(refpunkt);

  long double refdlug = (refpunkt - refpunkt2).length();
  long double refdlug2 = refdlug * 0.01;
  long double refdlug3 = refdlug2 * refdlug2;

  short isEnd = 0;
  long int stashcount = 0;
  long int stashcount2 = 0;

  do {
    step_counter++;
    current_point = fakeDoStep(*this, current_point, h, radius);

    if (step_counter == 100) {
      distance_after_100 = (current_point - refpunkt).squareLenght();
    }

    if (step_counter > 100) {
      distance_from_100 = (current_point - refpunkt).squareLenght();
      if (distance_from_100 <
          distance_after_100 * 0.0001) { // we are close, finish computation
        isEnd = 1;
        continue;
      }
    }

    if (step_counter ==
        20000 * steps_multiplier *
        steps_multiplier) { // too many steps, finish computation
      isEnd = 2;
      continue;
    }

    if (step_counter % 10 == 0) {
      if ((current_point - points.back()).squareLenght() > refdlug3) {
        points.push_back(current_point);
        stashcount++;
      }
    }

    if (step_counter % 10000 == 0) {
      checkIfHole(stashcount, stashcount2, isEnd);
    }
    if (step_counter % 1000 == 0) {
      checkIfDistanceMakesSence(odsuw3, refdlug, current_point, isEnd,
                                refpunkt2);
    }
  } while (isEnd == 0);
  points.push_back(refpunkt); // close image
  resultPoints = points;

  if (isEnd == 1) {
    return true;
  } else {
    return false;
  }
}

void map::printMapMatrix() {
  char buffer[1000];
  write_log("begin of function");
  int n = this->fun_matrix.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      complex value = this->fun_matrix.coef[i][j];
      if (value != complex(0,0)) {
        sprintf(buffer, "%d %d %lf %lf", i, j, value.real(), value.imag());
        write_log(buffer);
      }
    }
  }
  sprintf(buffer, "end of function");
  write_log(buffer);
}

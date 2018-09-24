#include "matbase.h"
#include <cmath>

/// program contains neccessary mathematical objects
int seed = time(NULL);

double RandomValue() {
  if (seed) {
    std::srand(time(NULL));
    seed = false;
  }
  auto rand = (std::rand() % 2000000000) * 0.00000001 - 10.0;
  return rand;
}

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

complex empty_fun(complex, complex) { return complex(0.0, 0.0); }

complex empty_fun2(complex) { return complex(0.0, 0.0); }

inline fourvector operator*(long double d, fourvector T) {
  return fourvector(T.x() * d, T.y() * d, T.z() * d, T.u() * d);
}

inline long double operator*(fourvector A, fourvector B) {
  return A.x() * B.x() + A.y() * B.y() + A.z() * B.z() + A.u() * B.u();
}

// [-3,0,0,0]
// [0,3,0,0]
// [1,0,0,0]
//

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
void checkIfHole(long int &stashcount, long int &stashcount2, short &isEnd) {
  if (stashcount < (stashcount2 + 2)) {
    write_log("We are in the hole.\n");
    isEnd = 4;
  }
  stashcount2 = stashcount;
}

void checkIfDistanceMakesSence(long double &odsuw3, long double &refdlug,
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

long double fourvector::length() {
  return sqrt(wx * wx + wy * wy + wz * wz + wu * wu);
}

fourvector map::trajectory(fourvector K) {
  complex z1(K.x(), K.y());
  complex z2(K.z(), K.u());

  complex poch1 = fun_matrix.giveDx().compute(z1, z2);
  complex poch2 = fun_matrix.giveDy().compute(z1, z2);

  fourvector direction1(poch1.re(), -poch1.im(), poch2.re(), -poch2.im());
  fourvector direction2(poch1.im(), poch1.re(), poch2.im(), poch2.re());
  fourvector Apar =
      ortogonal(K, direction1, direction2); // good direction to go

  return Apar;
}

// Newton method
complex fakeDoStep(parametr m, complex StartPoint, long double skok) {
  complex z0 = m.pdz(StartPoint);
  complex z1 = StartPoint + z0 * skok;
  return StartPoint + (z0 + m.pdz(z1)) * (0.5 * skok);
}

fourvector fakeDoStep(map m, fourvector StartPoint, long double skok,
                      long double R) {
  skok *= 2;
  fourvector z0 = m.trajectory(StartPoint);
  fourvector z1 = StartPoint + z0 * skok;
  fourvector result = StartPoint + (z0 + m.trajectory(z1)) * (0.5 * skok);

  return result * (R / result.length());
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
  // write_log("start point: " + z.to_string());
  counter = 1;
  ecounter = 1;
  scalar = r;
}

bool map::getToThePoint(long double r, fourvector &zstart) {

  write_log("getting start point");

  fourvector z;
  double scalar;
  int counter, extra_counter;
  long long all_steps = 0;
  restartSearch(counter, extra_counter, z, scalar, r);
  complex value = fun_matrix.compute(z);

  while (value.absolute() > 0.01) {
    counter++;
    all_steps++;
    complex poch1 = fun_matrix.giveDx().compute(z);
    complex poch2 = fun_matrix.giveDy().compute(z);

    value = fun_matrix.compute(z);

    fourvector gradient(poch1.re(), -1 * poch1.im(), poch2.re(),
                        -1 * poch2.im());
    gradient = gradient - ((gradient * z) * (1 / r) * z);

    // write_log("value of f " + value.to_string() +
    //          " extra = " + std::to_string(extra_counter));
    extra_counter++;

    if (extra_counter > 10) {
      restartSearch(counter, extra_counter, z, scalar, r);
    }

    if (gradient.squareLenght() < 0.01 && value.absolute() > 0.01) {
      write_log("to small gradient");
      restartSearch(counter, extra_counter, z, scalar, r);
    }

    fourvector zz = z - (gradient * scalar);

    zz = zz * (r / zz.length());

    complex valueZZ = fun_matrix.compute(zz);

    if (value.absolute() <= valueZZ.absolute()) {
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

  write_log("picked z: " + z.to_string() +
            ", steps: " + std::to_string(all_steps));
  write_log("z length " + std::to_string(z.length()));
  zstart = z;

  return true;
}

bool map::computePoints(std::vector<fourvector> &resultPoints, double radius,
                        long double h, double steps_multiplier) {
  fourvector Zstart;

  getToThePoint(radius, Zstart);

  std::vector<fourvector> points;

  long int step_counter = 0;

  long double distance_after_100 = 0.0;
  long double distance_from_100 = 100000.0;
  long double odsuw3 = 0.0;

  fourvector current_point = Zstart;
  fourvector refpunkt = Zstart;
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

std::string complex::to_string() {
  std::string result;
  result += std::to_string(this->re());
  if (this->im() != 0) {
    result += " + ";
    result += std::to_string(this->im());
    result += "i";
  }
  return result;
}

void map::printCoefMatrix() {
  int n = this->fun_matrix.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      write_log((this->fun_matrix.coef[i][j]).to_string() + " ");
    }
  }
}

void map::printFriendlyCoefMatrix() {
  int n = this->fun_matrix.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      complex value = this->fun_matrix.coef[i][j];
      //write_log("tego szukam: " + value.to_string());
      if (value != 0) {
        char buffer[1000];
        if(i != 0 && j != 0){
          sprintf(buffer, "(%s)x^%dy^%d + ", value.to_string().c_str(), i, j);
        }
        else if(j != 0)
          sprintf(buffer, "(%s)y^%d + ", value.to_string().c_str(), j);
        else if(i != 0){
          sprintf(buffer, "(%s)x^%d + ", value.to_string().c_str(), i);
        }
        else{
          sprintf(buffer, "(%s) + ", value.to_string().c_str());
        }
        write_log(buffer);

      }
    }
  }
}

bool parametr::computePoints(std::vector<fourvector> &resultPoints,
                             double radius, long double h,
                             double steps_multiplier) {
  complex Zstart;
  int count = 0;
  do {
    count++;
    Zstart = complex(RandomValue(), RandomValue());
    write_log(Zstart.to_string());
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
      checkIfHole(stashcount, stashcount2, isEnd);
    }
    if (step_counter % 1000 == 0) {
      checkIfDistanceMakesSence(odsuw3, refdlug, abyraz, isEnd, refpunkt2);
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

map::map() : function(QString("map:random")) {
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
      sprintf(buffer, "{{%d,%d}, %s}", i, j, rand.to_string().c_str());
      name += buffer;
    }
  }
  secret_name = name;
}

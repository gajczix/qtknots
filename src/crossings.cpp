//
// Created by Krystyna Gajczyk on 22/04/18.
//

#include "crossings.h"
#include <cassert>
#include <cmath>

#define PLUS_COLOR "#f46b42"
#define MINUS_COLOR "#2f38e2"
#define HALF_STEP 0.4
#define HALF_BEGIN 0.6
#define ALPHA 0
#define BETA 1

struct Point {
  double x;
  double y;

  Point(double x, double y) : x(x), y(y) {}

  std::string to_string() {
    return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) +
           ") ";
  }

  Point norm() {
    double lenght = sqrt(x * x + y * y);
    return Point(x / lenght, y / lenght);
  }

  Point operator-(Point b) { return Point(b.x - this->x, b.y - this->y); }
};

bool determinant(Point a1, Point a2, Point b1, Point b2);

double angle(Point a, Point b);

double third_dimen(fourvector begin, fourvector end, double scalar, double R);

Point intersectionPoint(Point point, std::pair<double, double> pair);

std::vector<std::set<int>> getSeifertCircles(std::vector<PDCrossing> PDCode);

bool inInterval(Point a, Point x1, Point x2) {
  return (a.x * (x1.y - x2.y) + x1.x * (x2.y - a.y) + x2.x * (a.y - x1.y) <
              0.0001 &&
          std::min(x1.x, x2.x) <= a.x && std::max(x1.x, x2.x) >= a.x);
}

std::pair<double, double> giveIntersectVector(Point p1, Point p2, Point q1,
                                              Point q2) {
  Point vectorQ(q2.x - q1.x, q2.y - q1.y);
  Point vectorP(p2.x - p1.x, p2.y - p1.y);

  if (vectorP.norm().x == vectorQ.norm().x &&
      vectorP.norm().y == vectorQ.norm().y) {
    return std::make_pair(-1, -1);
  }

  double beta = ((q1.x - p1.x) * vectorP.y - (q1.y - p1.y) * vectorP.x) /
                (vectorP.x * vectorQ.y - vectorQ.x * vectorP.y);
  double alfa = (q1.y + beta * vectorQ.y - p1.y) / vectorP.y;

  return std::make_pair(alfa, beta);
}

Point proj2d(fourvector punkt, double R) {
  Point p(punkt.x() / (punkt.z() + R), punkt.y() / (punkt.z() + R));
  return p;
}
static int counter = 0;
void printInterval(Point a, Point b) {
  write_log("\\draw[name path=P" + std::to_string(counter) + "]" +
            a.to_string() + " -- " + b.to_string() + " ;");
  counter++;
}

std::string preCrossing::to_string() {
  char buffer[1000];
  sprintf(buffer, "(%d, %d, a:%.4f, b:%.4f)", i, j, skalary.first,
          skalary.second);
  return buffer;
}

std::vector<preCrossing> giveAllIntersections(std::vector<fourvector> data,
                                              double R) {
  unsigned int n = data.size();

  std::vector<preCrossing> result;

  counter = 0;
  for (unsigned int i = 0; i < n - 2; i++) {
    for (unsigned int j = i + 2; j < n - 1; j++) {
      if (i == 0 &&
          (j == n - 2)) { // TODO: make it universal for multiple parts
        continue;
      }
      Point i1 = proj2d(data[i], R);
      Point i2 = proj2d(data[i + 1], R);
      Point j1 = proj2d(data[j], R);
      Point j2 = proj2d(data[j + 1], R);

      std::pair<double, double> ress = giveIntersectVector(i1, i2, j1, j2);

      if (ress.first >= 0 && ress.second >= 0 && ress.first <= 1 &&
          ress.second <= 1) {
        double yi = third_dimen(data[i], data[i + 1], ress.first, R);
        double yj = third_dimen(data[j], data[j + 1], ress.second, R);

        int isPositive =
            yi < yj ? determinant(i1, i2, j1, j2) : determinant(j1, j2, i1, i2);
        double angle_i_to_j = angle(i2 - i1, j2 - j1);

        result.push_back(
            preCrossing(i, j, ress, isPositive, yi >= yj, angle_i_to_j));
      }
    }
  }
  return result;
}

bool determinant(Point a1, Point a2, Point b1, Point b2) {
  Point a(a2.x - a1.x, a2.y - a1.y);
  Point b(b2.x - b1.x, b2.y - b1.y);
  return a.x * b.y - a.y * b.x <= 0;
}

double third_dimen(fourvector begin, fourvector end, double scalar, double R) {
  double beginU = begin.u();
  double endU = end.u();
  double beginZ = begin.z();
  double endZ = end.z();

  double e = beginU + scalar * (endU - beginU);
  double m = beginZ + scalar * (endZ - beginZ);

  return (-1 * (R - e)) / (m + R);
}

PDCrossing mapCrossings(PDCrossing cros, std::map<double, double> &mapa) {
  return PDCrossing(mapa[cros.i1], mapa[cros.i2], mapa[cros.i3], mapa[cros.i4],
                    cros.angle);
}

double angle(Point a, Point b) {
  a = a.norm();
  b = b.norm();

  double dot =
      a.x * b.x + a.y * b.y; // dot product between [x1, y1] and [x2, y2]
  double det = a.x * b.y - a.y * b.x; // determinant
  double angle = atan2(det, dot);     // atan2(y, x) or atan2(sin, cos);
  return angle;
}

void updateAngleMap(std::map<int, double> &angle_map, int counter,
                    double &begin, double &end, std::vector<fourvector> data,
                    double R) {
  double sum = 0;

  if (begin > end) {
    for (int i = 0; i < end; i++) {
      Point p1 = proj2d(data[i], R);
      Point p2 = proj2d(data[i + 1], R);
      Point p3 = proj2d(data[i + 2], R);

      sum += angle(p2 - p1, p3 - p2);
    }
    end = data.size();
  }
  for (int i = begin; i < end - 2; i++) {
    Point p1 = proj2d(data[i], R);
    Point p2 = proj2d(data[i + 1], R);
    Point p3 = proj2d(data[i + 2], R);

    sum += angle(p2 - p1, p3 - p2);
  }
  angle_map[counter] = sum;
}

PDCrossing computePDCrossing(preCrossing cros,
                             std::vector<std::pair<int, int>> shifts) {
  double i = cros.i;
  double j = cros.j;
  double i_n = 1;
  double j_n = 1;
  for (auto shift : shifts) {
    if (shift.first == 0) {
      i_n = HALF_STEP;
      if (shift.second == 1) {
        i += HALF_BEGIN;
      }
    } else {
      j_n = HALF_STEP;
      if (shift.second == 1) {
        j += HALF_BEGIN;
      }
    }
  }
  if (cros.is_i_higher) {
    if (cros.positive) {
      return PDCrossing(j, i + i_n, j + j_n, i, -1 * cros.i_to_j_angle);
    } else {
      return PDCrossing(j, i, j + j_n, i + i_n, -1 * cros.i_to_j_angle);
    }
  } else {
    if (cros.positive) {
      return PDCrossing(i, j + j_n, i + i_n, j, cros.i_to_j_angle);
    } else {
      return PDCrossing(i, j, i + i_n, j + j_n, cros.i_to_j_angle);
    }
  }
}

std::map<int, GaussCrossing> computeExtendedGauss(std::vector<fourvector> data,
                                                  double R) {
  std::map<int, GaussCrossing> result;
  std::map<std::pair<int, int>, int> mapa_wezly;

  std::map<GaussCrossing, int> wezly_znaki;
  int counter = 1;

  std::vector<preCrossing> crossings = giveAllIntersections(data, R);

  for (auto elem : crossings) {
    int i = elem.i;
    int j = elem.j;
    std::pair<int, int> intersectVector = elem.skalary;

    double yi = third_dimen(data[i], data[i + 1], intersectVector.first, R);
    double yj = third_dimen(data[j], data[j + 1], intersectVector.second, R);

    if (fabs(yi - yj) < 0.000001) {
      write_log(std::to_string(yi) + " " + std::to_string(yj));
      write_log("crossing in 3 dimensions, end of computation");
      return result; // empty_fun mapa
    }

    int wezel;
    std::pair<int, int> key = std::make_pair(std::min(i, j), std::max(i, j));
    if (mapa_wezly.find(key) != mapa_wezly.end()) {
      wezel = mapa_wezly[key];
    } else {
      mapa_wezly[key] = counter;
      wezel = counter;
      counter++;
    }

    if (i < j) {
      result.emplace(i, GaussCrossing(wezel, yi > yj));
      result.emplace(j, GaussCrossing(wezel, elem.positive));
    } else {
      result.emplace(i, GaussCrossing(wezel, yj > yi));
      result.emplace(j, GaussCrossing(wezel, elem.positive));
    }
  }
  return result;
}

void printCrossings(std::map<int, GaussCrossing> mapa) {
  FILE *file = fopen("crossings.txt", "w");
  for (auto &elem : mapa) {
    fprintf(file, "%d %d\n", elem.second.interval, elem.second.higher);
  }
  fclose(file);
}

void printGraph(graph_type graph, std::string name) {
  std::string file_name = "graphs/" + name + ".dot";
  FILE *file = fopen(file_name.c_str(), "w");
  fprintf(file, "graph \"\" {\ngraph [size=\"3,3!\",ratio=\"1\"];\n");
  int i = 0;
  for (auto elem = graph.signs.begin(); elem != graph.signs.end();
       elem++, i++) {
    fprintf(file, "%d [color=\"%s\"];\n", i,
            (*elem > 0) ? PLUS_COLOR : MINUS_COLOR);
  }

  i = 0;
  for (auto &elem : graph.edges) {
    for (auto &row : elem) {
      if (i < row.first)
        fprintf(file, "%d -- %d [color=\"%s\"];\n", i, row.first,
                (row.second > 0) ? PLUS_COLOR : MINUS_COLOR);
    }
    i++;
  }
  fprintf(file, "}\n");
  fclose(file);
}

void printPDCode(std::vector<PDCrossing> PDs, std::string name) {
  std::string file_name = "pdcodes/" + name + ".py";
  FILE *file = fopen(file_name.c_str(), "w");

  fprintf(file,
          "#!/usr/bin/env python2.7\n"
          "\n"
          "import sys\n"
          /* "sys.path.append('/Users/krystynka/miniconda2/lib/python2.7/" */
          /* "site-packages')\n" */
          "import snappy\n"
          "\n"
          "a = snappy.Link([");

  int n = PDs.size();
  for (int i = 0; i < n; i++) {
    PDCrossing PD = PDs[i];
    fprintf(file, "(%d, %d, %d, %d)", (int)PD.i1, (int)PD.i2, (int)PD.i3,
            (int)PD.i4);

    if (i != n - 1) {
      fprintf(file, ",\n");
    }
  }
  std::string commands = "])\n"
                         "b = a.view()\n"
                         "print b\n"
                         "b.save_as_svg('./pdpictures/" +
                         name + ".svg', None)";
  fprintf(file, "%s", commands.c_str());
  fclose(file);
}

int findSeifertCircle(int elem, std::vector<std::set<int>> seiferCircles) {
  for (unsigned int i = 0; i < seiferCircles.size(); i++) {
    auto circle = seiferCircles[i];
    if (circle.find(elem) != circle.end()) {
      return i;
    }
  }
  return -1;
}

int bigger(int a, int b, int n) {
  if (std::min(a, b) == 0 && std::max(a, b) == n - 1) {
    return 0;
  } else {
    return std::max(a, b);
  }
}

int smaller(int a, int b, int n) {
  if (std::min(a, b) == 0 && std::max(a, b) == n - 1) {
    return n - 1;
  } else {
    return std::min(a, b);
  }
}

std::vector<int> getSigns(std::vector<PDCrossing> PDCode,
                          std::map<int, double> angle_map,
                          std::vector<std::set<int>> seiferCircles) {
  int n = seiferCircles.size();
  std::vector<double> sums(n);
  std::vector<int> signs(n);

  for (int i = 0; i < n; i++) {
    for (auto interval : seiferCircles[i]) {
      sums[i] += angle_map[interval];
    }
  }

  for (unsigned int i = 0; i < PDCode.size(); i++) {
    int num1 = findSeifertCircle(PDCode[i].i1, seiferCircles);
    int num3 = findSeifertCircle(PDCode[i].i3, seiferCircles);

    sums[num1] += PDCode[i].angle;
    sums[num3] -= PDCode[i].angle;
  }

  for (int i = 0; i < n; i++) {
    write_log("sum is " + std::to_string(sums[i]));
    if (sums[i] > 0) {
      signs[i] = 1;
    } else {
      signs[i] = -1;
    }
  }
  return signs;
}

graph_type generateGraphFromPDCode(std::vector<PDCrossing> PDCode,
                                   std::map<int, double> angle_map) {
  std::vector<std::set<int>> seiferCircles = getSeifertCircles(PDCode);
  int n = seiferCircles.size(); // number of vertices in graph

  std::vector<std::vector<std::pair<int, int>>> result(
      n, std::vector<std::pair<int, int>>());

  std::vector<int> signs;

  if (n == 0) {
    return graph_type(result, signs);
  }

  signs = getSigns(PDCode, angle_map, seiferCircles);

  for (auto cros : PDCode) {
    int elem1 = cros.i1;
    int elem2 = cros.i3;

    int k = findSeifertCircle(elem1, seiferCircles);
    int j = findSeifertCircle(elem2, seiferCircles);

    bool isPositive = bigger(cros.i2, cros.i4, PDCode.size() * 2) == cros.i2;

    result[k].emplace_back(j, isPositive);
    result[j].emplace_back(k, isPositive);
  }
  return graph_type(result, signs);
}

std::vector<std::set<int>> getSeifertCircles(std::vector<PDCrossing> PDCode) {
  int n = PDCode.size() * 2;
  std::vector<int> path(n, -1);

  for (auto crossing : PDCode) {
    /// i1 enters crossing and turns according to orientation of
    /// ortogonal way - going to maximum of i2 and i4.
    path[crossing.i1] = bigger(crossing.i2, crossing.i4, n);
    write_log(std::to_string(crossing.i1) + " -> " +
              std::to_string(bigger(crossing.i2, crossing.i4, n)));
    /// from oposite site, lower from i2 and i4 continues to i3.
    path[smaller(crossing.i2, crossing.i4, n)] = crossing.i3;
    write_log(std::to_string(smaller(crossing.i2, crossing.i4, n)) + " -> " +
              std::to_string(crossing.i3));
  }
  write_log("after " + std::to_string(n));

  int component_counter = 0;
  std::vector<bool> visited(n, false);
  std::vector<std::set<int>> result;

  write_log("testing1");
  for (int i = 0; i < n; i++) {
    if (visited[i] == true) {
      continue;
    }
    int current = i;

    result.push_back(std::set<int>());
    result[component_counter].insert(i);
    visited[i] = true;
    write_log("testing2 " + std::to_string(current) +
              std::to_string(path.size()));
    if (path[current] == -1) {
      write_log("problem on graph generation");
      return std::vector<std::set<int>>();
    }

    write_log("testing3");
    while (visited[path[current]] == false) {
      current = path[current];
      write_log("testing2 " + std::to_string(current));
      if (current == -1) {
        write_log("problem on graph generation");
        return std::vector<std::set<int>>();
      }
      result[component_counter].insert(current);
      visited[current] = true;
    }

    component_counter++;
  }
  write_log("there are " + std::to_string(result.size()) + " Seifert circles");
  return result;
}

std::map<int, std::vector<std::pair<int, int>>>
giveBadIndexes(std::vector<preCrossing> &toFix,
               std::set<std::tuple<int, int, int, int>> problematic_indexes) {
  std::map<int, std::vector<std::pair<int, int>>> result;
  for (auto &problem : problematic_indexes) {
    int idx1 = std::get<0>(problem);
    int idx2 = std::get<1>(problem);
    preCrossing cand1 = toFix[idx1];
    preCrossing cand2 = toFix[idx2];

    int scal1 = std::get<2>(problem);
    int scal2 = std::get<3>(problem);

    double s1 = (scal1 == 0 ? cand1.skalary.first : cand1.skalary.second);
    double s2 = (scal2 == 0 ? cand2.skalary.first : cand2.skalary.second);

    if (result.find(idx1) == result.end()) {
      result[idx1] = std::vector<std::pair<int, int>>();
    }
    if (result.find(idx2) == result.end()) {
      result[idx2] = std::vector<std::pair<int, int>>();
    }

    if (s1 < s2) {
      result[idx1].push_back(std::make_pair(0, scal1));
      result[idx2].push_back(std::make_pair(1, scal2));
    } else {
      result[idx1].push_back(std::make_pair(1, scal1));
      result[idx2].push_back(std::make_pair(0, scal2));
    }
  }
  // TODO: print results

  return result;
}

void updateBegins(std::vector<double> &begins, double i, double j, double i_n,
                  double j_n) {
  begins.push_back(i);
  begins.push_back(i + i_n);
  begins.push_back(j);
  begins.push_back(j + j_n);
}

void fixBadIndex(std::vector<std::pair<int, int>> fixes,
                 std::vector<std::pair<int, int>> &changes, double &beg1,
                 double &beg2, double &step1, double &step2) {
  for (auto fix : fixes) {
    int order = fix.first;
    int scalar = fix.second;

    if (order == 0) { // first interval
      if (scalar == ALPHA) {
        step1 = HALF_STEP;
        changes.push_back({ALPHA, -1});
      } else {
        step2 = HALF_STEP;
        changes.push_back({BETA, -1});
      }
    } else { // second interval
      if (scalar == ALPHA) {
        beg1 += HALF_BEGIN;
        step1 = HALF_STEP;
        changes.push_back({ALPHA, 1});
      } else {
        beg2 += HALF_BEGIN;
        step2 = HALF_STEP;
        changes.push_back({BETA, 1});
      }
    }
  }
}

std::set<std::tuple<int, int, int, int>>
findDuplicateIndexes(std::vector<preCrossing> &preCrossings) {
  std::set<std::tuple<int, int, int, int>> problematic_indexes;
  std::map<int, std::pair<int, int>> distinct_begins;

  int i = 0;
  for (auto preC : preCrossings) {
    if (distinct_begins.find(preC.i) != distinct_begins.end()) {
      auto a = distinct_begins[preC.i];
      problematic_indexes.emplace(a.first, i, a.second, ALPHA);
    }
    if (distinct_begins.find(preC.j) != distinct_begins.end()) {
      auto a = distinct_begins[preC.j];
      problematic_indexes.emplace(a.first, i, a.second, BETA);
    }
    distinct_begins[preC.i] = std::make_pair(i, ALPHA);
    distinct_begins[preC.j] = std::make_pair(i, BETA);

    i++;
  }
  if (problematic_indexes.size() > 0) {
    write_log("detected some problematic indexes: ");
    for (auto elem : problematic_indexes) {
      write_log(std::to_string(std::get<0>(elem)) + ": " +
                preCrossings[std::get<0>(elem)].to_string() + "     " +
                std::to_string(std::get<1>(elem)) + ": " +
                preCrossings[std::get<1>(elem)].to_string());
    }
  }
  return problematic_indexes;
}

std::string PDCrossing::to_string() {
  char buffer[100];
  sprintf(buffer, "(%.2f, %.2f, %.2f, %.2f)", i1, i2, i3, i4);
  return buffer;
}

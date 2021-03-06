//
// Created by Krystyna Gajczyk on 06/05/18.
//

#include "Knot.h"

bool isNewComponent(std::vector<fourvector> &component,
                    std::vector<fourvector> &representation);

Knot::Knot(double radius, function *function) : R(radius), func(function) {
  bool zamk;
  double siz;
  std::vector<fourvector> wek;
  std::vector<fourvector> emptywek;
  std::set<int> sizes_of_wek;
  std::vector<fourvector> all_wek;
  std::vector<fourvector> representation;
  name = function->NameOfFunction;
  int last_size = 0;

  write_log("knot initialization");
  srand(time(NULL));

  int componentCounter = 0;
  /// We look for multiple components and starting process multiple times to
  /// find all of them.
  /// Number 10 here is random.
  for (int i = 0; i < 1; i++) { // TODO: change for 10
    wek = emptywek;
    zamk = func->computePoints(wek, R, 0.001, R);
    // TODO: make it faster by detecting old components after starting point.
    siz = double(wek.size());

    /// If component was found before, continue searching for new ones.
    if (!isNewComponent(wek, representation)) {
      continue;
    } else {
      componentCounter++;
    }

    if (componentCounter > 1) {
      write_log("handeling multiple components not implemented");
      // exit(0);
      // TODO: implement
    }

    if (!zamk) {
      write_log("Error while computing knot");
      exit(0);
    }

    all_wek.insert(all_wek.end(), wek.begin(), wek.end());

    sizes_of_wek.insert(siz + last_size);
    last_size += siz;
  }
  this->wek = all_wek;
}

int Knot::giveIntersectionNumber() {
  int wynik = giveAllIntersections(this->wek, this->R).size();
  write_log("wynik dla R=" + std::to_string(R) + " to " +
            std::to_string(wynik));
  return wynik;
}

Knot::Knot(std::vector<fourvector> data, std::string name)
    : R(1), wek(data), name(name) {}

std::vector<PDCrossing> Knot::givePDCode(std::map<int, double> &angle_map) {
  std::vector<double> begins;

  /// First we get all intersections
  std::vector<preCrossing> preCrossings =
      giveAllIntersections(this->wek, this->R);

  std::vector<PDCrossing> result;
  if (preCrossings.size() == 0) {
    return result;
  }

  auto problematic_indexes = findDuplicateIndexes(preCrossings);
  auto toFix = giveBadIndexes(preCrossings, problematic_indexes);

  std::vector<PDCrossing> notMappedCrossings;
  /// For every intersection, begining and end are memorized.
  int i = 0;
  for (auto elem : preCrossings) {
    std::vector<std::pair<int, int>> changes;
    double beg1 = elem.i, beg2 = elem.j, step1 = 1, step2 = 1;

    if (toFix.find(i) != toFix.end()) {
      fixBadIndex(toFix[i], changes, beg1, beg2, step1, step2);
    }
    notMappedCrossings.push_back(computePDCrossing(elem, changes));
    updateBegins(begins, beg1, beg2, step1, step2);
    i++;
  }
  /// After sorting we map numbers to be consecutive.
  std::sort(begins.begin(), begins.end());
  std::map<double, double> mapa = assignMappings(begins, angle_map);

  for (auto elem : notMappedCrossings) {
    result.push_back(mapCrossings(elem, mapa));
    // write_log(elem.to_string() + "   -----> " +
    //            mapCrossings(elem, mapa).to_string());
  }

  return result;
}

/**
 * Function to check if component is a new component of a link. Compares all
 * elements of candidate
 * with representation of older components. In case points are relatively close,
 * we stop computation.
 * @param component candidate to check
 * @param representation representation of all components that were found
 * already
 * @return true if component is new, otherwise false.
 */
bool isNewComponent(std::vector<fourvector> &component,
                    std::vector<fourvector> &representation) {
  for (auto rep : representation) {
    for (auto elem : component) {
      if ((rep - elem).squareLenght() < 0.01) { // TODO: scale to size of step
        return false;
      }
    }
  }
  representation.insert(representation.end(), component.begin(),
                        component.end());
  return true;
}

std::map<double, double>
Knot::assignMappings(std::vector<double> &begins,
                     std::map<int, double> &angle_map) {
  std::map<double, double> mapa;

  int n = begins.size();
  mapa[begins[0]] = 0;
  mapa[begins[n - 1]] = 0;

  int map_counter = 1;

  for (int i = 1; i < n - 1; i = i + 2) {
    mapa[begins[i]] = map_counter;
    mapa[begins[i + 1]] = map_counter;

    updateAngleMap(angle_map, map_counter, begins[i], begins[i + 1], this->wek,
                   this->R);

    map_counter++;
  }
  return mapa;
}

void Knot::dumpPoints() {
  std::cout << "points begin:\n";
  for (fourvector elem : this->wek) {
    char buffer[100];
    sprintf(buffer, "%Lf %Lf %Lf %Lf\n", elem.x(), elem.y(), elem.z(),
            elem.u());
    std::cout << buffer;
  }
  std::cout << "points end\n\n";
}

std::vector<fourvector> parseKnotFromString(std::string input) {
  std::string line;
  std::istringstream f(input);
  std::vector<fourvector> points;
  while (std::getline(f, line)) {
    double x, y, z, u;
    sscanf(line.c_str(), "%lf %lf %lf %lf", &x, &y, &z, &u);
    points.emplace_back(x, y, z, u);
  }
  return points;
}

map parseMapFromString(std::string input) {
  std::string line;
  std::istringstream f(input);
  std::vector<std::pair<std::pair<int, int>, complex>> coefficient;
  std::string name;
  std::getline(f, name);
  while (std::getline(f, line)) {
    int i, j;
    double x, y;
    sscanf(line.c_str(), "%d %d %lf %lf", &i, &j, &x, &y);
    coefficient.emplace_back(
        std::make_pair(std::make_pair(i, j), complex(x, y)));
  }
  return map(coefficient, name);
}

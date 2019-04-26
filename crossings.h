/** @file */
//
// Created by Krystyna Gajczyk on 22/04/18.
//

#ifndef QTKNOTS_PRZECIECIA_H
#define QTKNOTS_PRZECIECIA_H
#include "math_basics/matbase.h"
#include "log.h"
#include <map>
#include <set>
#include <string>

/**
 * structure to represent graph. It consist of 2 elements:
 * - signs of vertices
 * - edges with signs
 */
struct graph_type {
  std::vector<std::vector<std::pair<int, int>>> edges;
  std::vector<int> signs;

  graph_type(std::vector<std::vector<std::pair<int, int>>> edges,
             std::vector<int> signs)
      : edges(edges), signs(signs) {}
};

/**
 * Helper structure to represent crossing before it is turn
 * to PD Code of Gauss Code.
 */
struct preCrossing {
  int i;
  int j;
  std::pair<double, double> skalary;
  bool positive;
  bool is_i_higher;
  double i_to_j_angle;

  preCrossing(int i, int j, std::pair<double, double> skalary, int positive,
              bool is_i_higher, double i_to_j_angle)
      : i(i), j(j), skalary(skalary), positive(positive),
        is_i_higher(is_i_higher), i_to_j_angle(i_to_j_angle) {}

  std::string to_string();
};
/**
 * Structure to represent crossing in Gauss Code
 */
struct GaussCrossing {
  int interval;
  bool higher;

  GaussCrossing(int interval, bool higher)
      : interval(interval), higher(higher) {}
};

/**
 * Structure to represent crossing in PD Code
 */
struct PDCrossing {
  double i1;
  double i2;
  double i3;
  double i4;
  double angle;

  PDCrossing(double i1, double i2, double i3, double i4, double angle)
      : i1(i1), i2(i2), i3(i3), i4(i4), angle(angle) {}

  std::string to_string();
};

/**
 * Function to compute crossings from given data. It takes all intervals in
 * data and computes point where they will cross. If this point is
 * within intervals, it is added to result.
 * @param data input data in fourvector format.
 * @param R radius of sphere.
 * @return detected crossings in format of preCrossing.
 */
std::vector<preCrossing> giveAllIntersections(std::vector<fourvector> data,
                                              double R);

/**
 * TODO: analyze this case
 * @param dane
 * @param R
 * @return
 */
std::map<int, GaussCrossing> computeExtendedGauss(std::vector<fourvector> dane,
                                                  double R);

/**
 * Function to generate graph from PDCode. It first checks for Seifert circles,
 * then analyse crossing to assign signs to edges. angle_map is needed to
 * determine signs of circles. Each sign is sum of angles on edges and
 * angles on crossings.
 * @param PDCode input data
 * @param angle_map hepler information about angles on each edge.
 * @return computed graph
 */
graph_type generateGraphFromPDCode(std::vector<PDCrossing> PDCode,
                                   std::map<int, double> angle_map);

/**
 * Function to compute PD crossing. It gets preCrossing to
 * convert with additional information about shifts needed.
 * Shifts needs to be applied if there are multiple crossings
 * within single interval.
 * @param cros preCrosing input information about crossing
 * @param shifts vector of changes needed to be applied
 * @return PDCrossing
 */
PDCrossing computePDCrossing(preCrossing cros,
                             std::vector<std::pair<int, int>> shifts = {});

/**
 * Function to update angle between begin and end interval of data.
 * It sums angles between all intervals and returns it.
 * @param angle_map Map to update.
 * @param counter PDCode number representing interval.
 * @param begin Begin interval.
 * @param end End interval.
 * @param data Data needed to calculate angles.
 */
void updateAngleMap(std::map<int, double> &angle_map, int counter,
                    double &begin, double &end, std::vector<fourvector> data,
                    double R);

/**
 * Function to renumber intervals so edges get consecutive numbers. All
 * information needed for mapping is in mapa
 * @param cros input crossing that needs to be renumbered.
 * @param mapa map of old to new numbers
 * @return new crossing with correct numbering
 */
PDCrossing mapCrossings(PDCrossing cros, std::map<double, double> &mapa);

void printCrossings(std::map<int, GaussCrossing> mapa);

void printGraph(graph_type graph, std::string name);

void printPDCode(std::vector<PDCrossing> PDs, function *pFunction);

std::map<int, std::vector<std::pair<int, int>>>
giveBadIndexes(std::vector<preCrossing> &toFix,
               std::set<std::tuple<int, int, int, int>> problematic_indexes);

void updateBegins(std::vector<double> &i, double j, double i1, double i2,
                  double i3);

void fixBadIndex(std::vector<std::pair<int, int>> toFix,
                 std::vector<std::pair<int, int>> &changes, double &beg1,
                 double &beg2, double &step1, double &step2);

std::set<std::tuple<int, int, int, int>>
findDuplicateIndexes(std::vector<preCrossing> &vector);

#endif // QTKNOTS_PRZECIECIA_H

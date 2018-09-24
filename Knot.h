//
// Created by Krystyna Gajczyk on 06/05/18.
//

#ifndef QTKNOTS_WEZEL_H
#define QTKNOTS_WEZEL_H

#include "crossings.h"
#include "matbase.h"
#include <set>

class Knot {
  std::vector<fourvector> wek;
  double R;
  function *func;

public:
  /**
   * Constructor of object Knot. It computes vector with point representation of
   * the knot.
   * @param radius - radius of the ball we intersect function with.
   * @param function - function used to compute knot.
   */
  Knot(double R, function *funkcja);

  /**
   * Function to compute numbers of crossings in knot's diagram.
   * @return number of crossings.
   */
  int giveIntersectionNumber();

  /**
   * Function to draw knot. By default it draws 3d knot, but can also
   * draw projection to 2d.
   * @param isProjection if true, draw 2d, else 3d.
   */
  void draw(bool isProjection = false);

  /**
   * Function to compute PD code.
   * @param data - vector with points
   * @param R - radius of sphere
   * @return - vector of PD code representing given data.
   */
  std::vector<PDCrossing> givePDCode(std::map<int, double> &angle_map);

private:
  std::map<double, double> assignMappings(std::vector<double> &begins,
                                          std::map<int, double> &angle_map);
};

#endif // QTKNOTS_WEZEL_H

#include <../Knot.h>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    return -1;
  }

  char *fileName = argv[1];
  std::ifstream ifs(fileName);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  std::vector<fourvector> points = parseKnotFromString(content);
  Knot newKnot(points, "customName");

  std::cout << "number of crossings: " << newKnot.giveIntersectionNumber()
            << std::endl;

  std::map<int, double> angle_map;
  auto PDCode = newKnot.givePDCode(angle_map);

  // std::string knotName = newKnot.getName();
  // printPDCode(PDCode, knotName);

  auto graph = generateGraphFromPDCode(PDCode, angle_map);

  // printGraph(graph, knotName);

  std::cout << "DN Index: " << doubleNegativeIndex(graph) << std::endl;

  return 0;
}

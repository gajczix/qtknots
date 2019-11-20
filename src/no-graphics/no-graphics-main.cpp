#include <../Knot.h>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 3) {
    return -1;
  }

  int mode = atoi(argv[1]);
  char *fileName = argv[2];

  std::ifstream ifs(fileName);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  Knot newKnot;

  if (mode == 0) { // knot from points
    newKnot = Knot(parseKnotFromString(content), "knotFromPoints");
  } else { // knot from function
    map fun = parseMapFromString(content);
    newKnot = Knot(1, &fun);
    newKnot.dumpPoints();
  }

  std::cout << "results for knot: " << newKnot.getName() << std::endl;
  std::cout << "number of crossings: " << newKnot.giveIntersectionNumber()
            << std::endl;

  std::map<int, double> angle_map;
  auto PDCode = newKnot.givePDCode(angle_map);

  auto graph = generateGraphFromPDCode(PDCode, angle_map);

  std::cout << "DN Index: " << doubleNegativeIndex(graph) << std::endl;

  return 0;
}

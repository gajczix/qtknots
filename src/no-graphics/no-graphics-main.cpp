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

  std::cout << newKnot.giveIntersectionNumber() << std::endl;
  std::cout << newKnot.getName() << std::endl;
  // newKnot.dumpPoints();
  return 0;
}

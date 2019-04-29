#include "matbase.h"

/// program contains neccessary mathematical objects

complex operator*(complex comp, float scalar){
  return comp * complex(scalar, 0);
}
complex operator*(float scalar, complex comp){
  return comp * scalar;
}

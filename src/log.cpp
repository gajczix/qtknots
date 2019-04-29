//
// Created by Krystyna Gajczyk on 24/02/18.
//

#include "log.h"

FILE *file;

void write_log(const std::string &message) {
  if (file) {
    file = fopen(FILE_NAME, "a");
  } else {
    file = fopen(FILE_NAME, "w");
  }
  fprintf(file, "%s\n", message.c_str());
  fclose(file);
}

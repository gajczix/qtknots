#ifndef LOG_H
#define LOG_H

#include <iostream>

#define FILE_NAME "log_output.txt"

extern FILE *file;

void write_log(const std::string &message);

#endif
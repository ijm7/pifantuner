#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define PIFANTUNERD_EXPORT __declspec(dllexport)
#else
  #define PIFANTUNERD_EXPORT
#endif

PIFANTUNERD_EXPORT void pifantunerd();
PIFANTUNERD_EXPORT void pifantunerd_print_vector(const std::vector<std::string> &strings);

#ifndef WCPPYUTIL_SCN_VERTEX_H
#define WCPPYUTIL_SCN_VERTEX_H

#include <vector>
#include <string>

namespace WCPPYUTIL {

typedef float FLOAT;

std::vector<FLOAT> SCN_Vertex(const std::string &module,
                              const std::string &function,
                              const std::string &weights,
                              const std::vector<FLOAT> &x,
                              const std::vector<FLOAT> &y,
                              const std::vector<FLOAT> &z,
                              const std::vector<FLOAT> &q,
                              const std::string &dtype="float32");
}
#endif
#ifndef WCPPYUTIL_SCN_VERTEX_H
#define WCPPYUTIL_SCN_VERTEX_H

#include <vector>
#include <string>

namespace WCPPYUTIL {

#define FLOAT float
std::vector<FLOAT> SCN_Vertex(const std::string &module,
                              const std::string &function,
                              const std::vector<FLOAT> &x,
                              const std::vector<FLOAT> &y,
                              const std::vector<FLOAT> &z,
                              const std::vector<FLOAT> &q);
}
#endif
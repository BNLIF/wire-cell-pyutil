#ifndef WCPPYUTIL_SCN_VERTEX_H
#define WCPPYUTIL_SCN_VERTEX_H

#include "WCPPYUTIL/config.h"

#include <vector>
#include <string>

namespace WCPPYUTIL {

    std::vector<FLOAT> SCN_Vertex(const std::string &module, const std::string &function, const std::string &weights,
                                  const std::vector<std::vector<FLOAT> > &input, const std::string &dtype = "float32",
                                  const bool verbose = false);
}
#endif
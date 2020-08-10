#ifndef WCPPYUTIL_READ_NUMPY_H
#define WCPPYUTIL_READ_NUMPY_H

#include <vector>
#include <string>

namespace WCPPYUTIL {

std::vector<float> read_npz(const std::string &module,
                              const std::string &function,
                              const std::string &path,
                              const std::string &key,
                              const int col);
}
#endif
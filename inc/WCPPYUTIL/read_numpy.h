#ifndef WCPPYUTIL_READ_NUMPY_H
#define WCPPYUTIL_READ_NUMPY_H

#include "WCPPYUTIL/config.h"

#include <vector>
#include <string>

namespace WCPPYUTIL {

    std::vector<FLOAT> read_npz(const std::string &module, const std::string &function, const std::string &path,
                                const std::string &key, const int col);
}
#endif
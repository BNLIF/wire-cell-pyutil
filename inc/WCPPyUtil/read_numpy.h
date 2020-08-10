#ifndef WCPPYUTIL_READ_NUMPY_H
#define WCPPYUTIL_READ_NUMPY_H

#include "WCPPyUtil/config.h"

#include <vector>
#include <string>

namespace WCPPyUtil {

    std::vector<FLOAT> read_npz(const std::string &module, const std::string &function, const std::string &path,
                                const std::string &key, const int col, const bool verbose = false);
}
#endif
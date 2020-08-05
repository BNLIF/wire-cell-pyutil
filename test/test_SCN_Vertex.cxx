#include "WCPPYUTIL/SCN_Vertex.h"

int main() {

  typedef float VType;

  std::vector<VType> x = {0.1, 1.1, 2.1};
  std::vector<VType> y = {0.2, 1.2, 2.2};
  std::vector<VType> z = {0.3, 1.3, 2.3};
  std::vector<VType> q = {1, 1, 1};

  const std::string module = "SCN_Vertex";
  const std::string function = "Test";
  const std::string weights = "weights.pth";

  auto vxt = WCPPYUTIL::SCN_Vertex(module, function, weights, x, y, z, q);

  return 0;
}
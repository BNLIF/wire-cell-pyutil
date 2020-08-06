#include "WCPPYUTIL/SCN_Vertex.h"

int main() {

  typedef float VType;

  std::vector<VType> x = {00.0, 01.0, 02.0};
  std::vector<VType> y = {10.0, 11.0, 12.0};
  std::vector<VType> z = {20.0, 21.0, 22.0};
  std::vector<VType> q = {1.0, 2.0, 3.0};

  const std::string module = "SCN_Vertex";
  const std::string function = "SCN_Vertex";
  const std::string weights = "/lbne/u/hyu/lbne/uboone/wire-cell-pydata/scn_vtx/t48k-m16-l5-lr5d-res0.5-CP24.pth";

  auto vxt = WCPPYUTIL::SCN_Vertex(module, function, weights, x, y, z, q);

  return 0;
}
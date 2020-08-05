#include <Python.h>

#include "WCPPYUTIL/SCN_Vertex.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#define DebugVar(x) std::cout << "DebugVar: " << #x << ": " << x << std::endl
#define DebugInf(x) std::cout << "DebugInf: " << x << std::endl

template <class VType>
void print(const std::vector<VType> &data) {
  for (auto v : data) std::cout << v << " ";
  std::cout << std::endl;
}

namespace WCPPYUTIL {
#define FLOAT float
std::vector<FLOAT> SCN_Vertex(const std::string &module,
                              const std::string &function,
                              const std::string &weights,
                              const std::vector<FLOAT> &x,
                              const std::vector<FLOAT> &y,
                              const std::vector<FLOAT> &z,
                              const std::vector<FLOAT> &q) {
  DebugInf("SCN_Vertex: start");
  DebugVar(module);
  DebugVar(function);
  DebugVar(weights);
  print<FLOAT>(x);
  print<FLOAT>(y);
  print<FLOAT>(z);
  print<FLOAT>(q);

  size_t npts = q.size();
  if (x.size() != npts || y.size() != npts || z.size() != npts) {
    throw std::runtime_error("input size unmatch");
  }

  PyObject *pName, *pWeights, *pModule, *pDict, *pFunc, *pValue;

  // Initialize the Python Interpreter
  Py_Initialize();

  // Build the name object
  pName = PyString_FromString(module.c_str());

  // weight path, needs to dec ref
  pWeights = PyString_FromString(weights.c_str());

  // Load the module object
  pModule = PyImport_Import(pName);
  if (!pModule) {
    throw std::runtime_error("import failed");
  }

  // pDict is a borrowed reference
  pDict = PyModule_GetDict(pModule);
  if (!pDict) {
    throw std::runtime_error("pDict failed");
  }

  // pFunc is also a borrowed reference
  pFunc = PyDict_GetItemString(pDict, function.c_str());
  if (!pDict) {
    throw std::runtime_error("pFunc failed");
  }

  if (PyCallable_Check(pFunc) != true) {
    throw std::runtime_error("pFunc not callable");
  }

  size_t input_size = npts * sizeof(FLOAT);
  auto pX = PyBytes_FromStringAndSize((const char *)x.data(), input_size);
  auto pY = PyBytes_FromStringAndSize((const char *)y.data(), input_size);
  auto pZ = PyBytes_FromStringAndSize((const char *)z.data(), input_size);
  auto pQ = PyBytes_FromStringAndSize((const char *)q.data(), input_size);
  if (PyBytes_Check(pX) != true || PyBytes_Check(pY) != true ||
      PyBytes_Check(pZ) != true || PyBytes_Check(pQ) != true) {
    throw std::runtime_error("bad pData");
  }

  DebugInf("PyObject_CallFunctionObjArgs: start");
  // std::cout << std::endl;  // TODO: this is somehow needed to avoid seg fault
  pValue = PyObject_CallFunctionObjArgs(pFunc, pWeights, pX, pY, pZ, pQ, NULL);
  DebugInf("PyObject_CallFunctionObjArgs: end");

  size_t ret_size = PyBytes_Size(pValue);
  DebugVar(ret_size);
  assert(ret_size % sizeof(FLOAT) == 0);
  std::vector<FLOAT> ret;
  ret.resize(ret_size / sizeof(FLOAT));
  memcpy((char *)ret.data(), (char *)PyBytes_AsString(pValue), ret_size);

  if (pX != NULL) {
    Py_DECREF(pX);
  }
  if (pY != NULL) {
    Py_DECREF(pY);
  }
  if (pZ != NULL) {
    Py_DECREF(pZ);
  }
  if (pQ != NULL) {
    Py_DECREF(pQ);
  }

  if (pValue != NULL) {
    printf("Return of call: ");
    print<FLOAT>(ret);
    Py_DECREF(pValue);
  } else {
    PyErr_Print();
  }

  // Clean up
  Py_DECREF(pModule);
  Py_DECREF(pName);
  Py_DECREF(pWeights);

  // Finish the Python Interpreter
  Py_Finalize();

  return ret;
}
}
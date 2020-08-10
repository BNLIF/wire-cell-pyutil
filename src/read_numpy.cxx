#include <Python.h>

#include "WCPPYUTIL/read_numpy.h"

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
std::vector<float> read_npz(const std::string &module,
                            const std::string &function,
                            const std::string &path, const std::string &key,
                            const int col) {
  DebugInf("read_npz: start");

  PyObject *pModule, *pFunc;

  // Initialize the Python Interpreter
  Py_Initialize();
  DebugInf("Py_Initialize: OK");

  // Build the name object
  auto pName = PyString_FromString(module.c_str());
  auto pPath = PyString_FromString(path.c_str());
  auto pKey = PyString_FromString(key.c_str());
  auto pCol = PyInt_FromLong((long)col);
  auto pDtype = PyString_FromString("float32");
  DebugInf("pVar: OK");

  // Load the module object
  pModule = PyImport_Import(pName);
  DebugInf("pModule: OK");
  if (!pModule) {
    throw std::runtime_error("import failed");
  }

  // pDict is a borrowed reference
  auto pDict = PyModule_GetDict(pModule);
  DebugInf("pDict: OK");
  if (!pDict) {
    throw std::runtime_error("pDict failed");
  }

  // pFunc is also a borrowed reference
  pFunc = PyDict_GetItemString(pDict, function.c_str());
  DebugInf("pFunc: OK");
  if (!pFunc) {
    throw std::runtime_error("pFunc failed");
  }

  if (PyCallable_Check(pFunc) != true) {
    throw std::runtime_error("pFunc not callable");
  }
  
  DebugInf("PyObject_CallFunctionObjArgs: start");
  auto pValue = PyObject_CallFunctionObjArgs(pFunc, pPath, pKey, pCol, pDtype, NULL);
  DebugInf("PyObject_CallFunctionObjArgs: end");

  size_t ret_size = PyBytes_Size(pValue);
  DebugVar(ret_size);
  assert(ret_size % sizeof(float) == 0);
  std::vector<float> ret;
  ret.resize(ret_size / sizeof(float));
  memcpy((char *)ret.data(), (char *)PyBytes_AsString(pValue), ret_size);

  if (pValue != NULL) {
    printf("Return of call: ");
    print<float>(ret);
    Py_DECREF(pValue);
  } else {
    PyErr_Print();
  }

  // Clean up
  Py_DECREF(pModule);
  Py_DECREF(pName);
  Py_DECREF(pPath);
  Py_DECREF(pKey);
  Py_DECREF(pCol);
  Py_DECREF(pDtype);

  // Finish the Python Interpreter
  Py_Finalize();

  return ret;
}
}
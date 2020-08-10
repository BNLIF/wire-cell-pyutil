#include <Python.h>

#include "WCPPyUtil/read_numpy.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#define DebugVar(x) std::cout << "DebugVar: " << #x << ": " << x << std::endl
#define DebugInf(x) std::cout << "DebugInf: " << x << std::endl

template <class VType>
void print(const std::vector<VType> &data)
{
    for (auto v : data) std::cout << v << " ";
    std::cout << std::endl;
}

namespace WCPPyUtil {
    std::vector<FLOAT> read_npz(const std::string &module, const std::string &function, const std::string &path,
                                const std::string &key, const int col, const bool verbose)
    {
        if (verbose) {
            DebugInf("read_npz: start");
        }

        PyObject *pModule, *pFunc;

        // Initialize the Python Interpreter
        Py_Initialize();
        if (verbose) {
            DebugInf("Py_Initialize: OK");
        }

        // Build the name object
        auto pName = PyString_FromString(module.c_str());
        auto pPath = PyString_FromString(path.c_str());
        auto pKey = PyString_FromString(key.c_str());
        auto pCol = PyInt_FromLong((long) col);
        auto pDtype = PyString_FromString("float32");
        if (verbose) {
            DebugInf("pVar: OK");
        }

        // Load the module object
        pModule = PyImport_Import(pName);
        if (verbose) {
            DebugInf("pModule: OK");
        }
        if (!pModule) {
            throw std::runtime_error("import failed");
        }

        // pDict is a borrowed reference
        auto pDict = PyModule_GetDict(pModule);
        if (verbose) {
            DebugInf("pDict: OK");
        }
        if (!pDict) {
            throw std::runtime_error("pDict failed");
        }

        // pFunc is also a borrowed reference
        pFunc = PyDict_GetItemString(pDict, function.c_str());
        if (verbose) {
            DebugInf("pFunc: OK");
        }
        if (!pFunc) {
            throw std::runtime_error("pFunc failed");
        }

        if (PyCallable_Check(pFunc) != true) {
            throw std::runtime_error("pFunc not callable");
        }

        auto pValue = PyObject_CallFunctionObjArgs(pFunc, pPath, pKey, pCol, pDtype, NULL);
        if (verbose) {
            DebugInf("PyObject_CallFunctionObjArgs: OK");
        }

        size_t ret_size = PyBytes_Size(pValue);
        if (verbose) {
            DebugVar(ret_size);
        }
        assert(ret_size % sizeof(FLOAT) == 0);
        std::vector<FLOAT> ret;
        ret.resize(ret_size / sizeof(FLOAT));
        memcpy((char *) ret.data(), (char *) PyBytes_AsString(pValue), ret_size);

        if (pValue != NULL) {
            if (verbose) {
                printf("Return of call: ");
                print<FLOAT>(ret);
            }
            Py_DECREF(pValue);
        }
        else {
            PyErr_Print();
        }

        // Clean up
        Py_DECREF(pModule);
        Py_DECREF(pName);
        Py_DECREF(pPath);
        Py_DECREF(pKey);
        Py_DECREF(pCol);
        Py_DECREF(pDtype);

        /*!
         * seg fault if do Py_Finalize, ref:
         * https://cdcvs.fnal.gov/redmine/projects/ubcv/repository/revisions/v08_00_00_dl_br/entry/ubcv/ubdlintegration/PyNet_SparseSSNet.cxx#L53
         * TODO: figure out how to fix this
         */
        // Py_Finalize();

        return ret;
    }
}  // namespace WCPPyUtil
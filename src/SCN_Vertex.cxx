#include <Python.h>

#include "WCPPYUTIL/SCN_Vertex.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#define DebugVar(x) //std::cout << "DebugVar: " << #x << ": " << x << std::endl
#define DebugInf(x) //std::cout << "DebugInf: " << x << std::endl

template <class VType>
void print(const std::vector<VType> &data)
{
    for (auto v : data) std::cout << v << " ";
    std::cout << std::endl;
}

namespace WCPPYUTIL {
    std::vector<FLOAT> SCN_Vertex(const std::string &module, const std::string &function, const std::string &weights,
                                  const std::vector< std::vector<FLOAT> > &input, const std::string &dtype, const bool verbose)
    {
        if (input.size() != 4) {
            throw std::runtime_error("input.size() != 4");
        }
        auto x = input[0];
        auto y = input[1];
        auto z = input[2];
        auto q = input[3];
        if (verbose) {
            DebugInf("SCN_Vertex: start");
            DebugVar(module);
            DebugVar(function);
            DebugVar(weights);
            DebugVar(dtype);
        }

        size_t npts = q.size();
        if (x.size() != npts || y.size() != npts || z.size() != npts) {
            throw std::runtime_error("input size unmatch");
        }

        PyObject *pName, *pWeights, *pDtype, *pModule, *pDict, *pFunc, *pValue;

        // Initialize the Python Interpreter
        Py_Initialize();

        // Build the name object
        pName = PyString_FromString(module.c_str());

        // weight path, needs to dec ref
        pWeights = PyString_FromString(weights.c_str());

        // weight path, needs to dec ref
        pDtype = PyString_FromString(dtype.c_str());

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
        auto pX = PyBytes_FromStringAndSize((const char *) x.data(), input_size);
        auto pY = PyBytes_FromStringAndSize((const char *) y.data(), input_size);
        auto pZ = PyBytes_FromStringAndSize((const char *) z.data(), input_size);
        auto pQ = PyBytes_FromStringAndSize((const char *) q.data(), input_size);
        if (PyBytes_Check(pX) != true || PyBytes_Check(pY) != true || PyBytes_Check(pZ) != true ||
            PyBytes_Check(pQ) != true) {
            throw std::runtime_error("bad pData");
        }

        pValue = PyObject_CallFunctionObjArgs(pFunc, pWeights, pX, pY, pZ, pQ, pDtype, NULL);
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
            if (verbose) {
                printf("SCN_Vertex: Return of call: ");
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
        Py_DECREF(pWeights);
        Py_DECREF(pDtype);

        /*!
         * seg fault if do Py_Finalize, ref:
         * https://cdcvs.fnal.gov/redmine/projects/ubcv/repository/revisions/v08_00_00_dl_br/entry/ubcv/ubdlintegration/PyNet_SparseSSNet.cxx#L53
         * TODO: figure out how to fix this
         */
        // Py_Finalize();

        return ret;
    }
}  // namespace WCPPYUTIL
// A sample of calling
// python functions from C code

#include <Python.h>

#include <iostream>
#include <vector>

#define Debug(x) std::cout << "Debug: " << #x << ": " << x << std::endl

template <class VType>
void print(const std::vector<VType> &data)
{
    for (auto v : data) std::cout << v << " ";
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue;

    if (argc < 3) {
        printf("Usage: exe_name python_source function_name\n");
        return 1;
    }

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString(argv[1]);

    // Load the module object
    pModule = PyImport_Import(pName);

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, argv[2]);

    // input data
    typedef int VType;
    // std::vector<VType> input = {44, 43};
    std::vector<VType> input = {atoi(argv[3]), atoi(argv[4])};
    std::cout << "C++ input: ";
    print<VType>(input);

    if (PyCallable_Check(pFunc)) {
        size_t input_size = input.size() * sizeof(VType);
        auto pData = PyBytes_FromStringAndSize((const char *) input.data(), input_size);
        assert(PyBytes_Check(pData) == true);

        // std::cout << std::endl;  // TODO: this is somehow needed to avoid seg
        // fault
        pValue = PyObject_CallFunctionObjArgs(pFunc, pData, NULL);

        size_t ret_size = PyBytes_Size(pValue);
        assert(ret_size % sizeof(VType) == 0);
        std::vector<VType> ret;
        ret.resize(ret_size / sizeof(VType));
        memcpy((char *) ret.data(), (char *) PyBytes_AsString(pValue), ret_size);

        if (pData != NULL) {
            Py_DECREF(pData);
        }

        if (pValue != NULL) {
            printf("Return of call: ");
            print<VType>(ret);
            Py_DECREF(pValue);
        }
        else {
            PyErr_Print();
        }
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

    return 0;
}
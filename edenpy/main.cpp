
#include <Python.h>
#include "edenpy.h"
#include "edencore/ver.h"
#include "edenanalytics/core/edenanalytics.h"


static PyObject* edenpy_version_wrapper(PyObject* self, PyObject* args) {
    return PyUnicode_FromString("0.2.3");
}

static PyObject* eden_core_version_wrapper(PyObject* self, PyObject* args) {
    return PyUnicode_FromString(eden::getVersion().c_str());
}

static PyObject* eden_analytics_version_wrapper(PyObject* self, PyObject* args) {
    return PyUnicode_FromString(eden::analytics::getVersion().c_str());
}
// Example with string input
static PyObject* reverse_string_wrapper(PyObject* self, PyObject* args) {
    const char* input_str;
    if (!PyArg_ParseTuple(args, "s", &input_str)) {
        return NULL;
    }

    // Calculate the length of the input string
    int len = strlen(input_str);

    // Allocate memory for the reversed string
    char* reversed_str = (char*)malloc((len + 1) * sizeof(char));
    if (reversed_str == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed");
        return NULL;
    }

    // Reverse the string
    for (int i = 0; i < len; ++i) {
        reversed_str[i] = input_str[len - i - 1];
    }
    reversed_str[len] = '\0';  // Null-terminate the string

    // Create a Python string object from the reversed C string
    PyObject* result = PyUnicode_FromString(reversed_str);

    // Free the allocated memory
    free(reversed_str);

    return result;
}

/**
 * Factorial wrapper.
 */

static PyObject* factorial_wrapper(PyObject* self, PyObject* args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    return PyLong_FromLong(factorial(n));
}

/**
 * Methods to export in the module.
 */
static PyMethodDef methods[] = {
    {"edenpy_version", edenpy_version_wrapper, METH_VARARGS, "Version of eden py"},
    {"eden_core_version", eden_core_version_wrapper, METH_VARARGS, "Version of eden core library"},
    {"eden_analytics_version", eden_analytics_version_wrapper, METH_VARARGS, "Version of eden analytics library"},
    {"reverse_string", reverse_string_wrapper, METH_VARARGS, "Reverse a string"},
    {"factorial", factorial_wrapper, METH_VARARGS, "Calculate the factorial of a number."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "edenpy",
    "A module for eden library.",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_edenpy(void) {
    return PyModule_Create(&module);
}

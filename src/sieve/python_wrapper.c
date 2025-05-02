#include <Python.h>
#include "sieve.h"
#include "sieve_exceptions.h"

void pythonExceptionHandler(const char* message, const char* errorType) {
//    PyObject* exType = PyExc_RuntimeError;
//    if (strcmp(errorType, "ValueError") == 0) exType = PyExc_ValueError;
    // Add more mappings if needed

//    PyErr_Format(exType, "%s: %s", errorType, message);
    return;
}

typedef struct {
    PyObject_HEAD
    SieveRange *ptr;
} PySieveRange;

// Deallocate
static void PySieveRange_dealloc(PySieveRange *self) {
    if (self->ptr) {
        freeSieveRange(self->ptr);
        free(self->ptr);
    }
    Py_TYPE(self)->tp_free((PyObject *) self);
}

// __str__ or __repr__
static PyObject *PySieveRange_repr(PySieveRange *self) {
    return PyUnicode_FromFormat("<PySieveRange range_size=%d(%d, %d)>", self->ptr->rangeSize, self->ptr->start,
                                self->ptr->end);
}

// Python type definition
static PyTypeObject PySieveRangeType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "sieve.PySieveRange",
        .tp_basicsize = sizeof(PySieveRange),
        .tp_dealloc = (destructor)PySieveRange_dealloc,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_doc = "Wrapper for MyStructure",
        .tp_repr = (reprfunc)PySieveRange_repr,
};

// The actual function
static PyObject *load(PyObject *self, PyObject *args) {
    char *path;
    if (!PyArg_ParseTuple(args, "s", &path)) {
        return NULL;
    }

    SieveRange range = *loadSieveRangeFromFile(path);

//    if (!range) {
//        PyErr_SetString(PyExc_RuntimeError, "Failed to load structure");
//        return NULL;
//    }

//    return PyLong_FromLong(result);

    PySieveRange* py_obj = PyObject_New(PySieveRange, &PySieveRangeType);
    py_obj->ptr = &range;
    return (PyObject*)py_obj;

}

// Method definitions
static PyMethodDef sieveMethods[] = {
        {"load", load, METH_VARARGS, "load primes"},
        {NULL, NULL, 0, NULL}  // Sentinel
};

// Module definition
static struct PyModuleDef sieve = {
        PyModuleDef_HEAD_INIT,
        "sieve",           // Module name
        NULL,                 // Optional docstring
        -1,                   // Size of per-interpreter state of the module
        sieveMethods
};

// Init function
PyMODINIT_FUNC PyInit_sieve(void) {

    return PyModule_Create(&sieve);
}

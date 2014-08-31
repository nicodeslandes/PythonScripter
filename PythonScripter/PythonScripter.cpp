// PythonScripter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>

#include "Python.h"

using namespace std;

void printObject(const char* name, PyObject* obj)
{
	cout << name << ": " << PyString_AsString(PyObject_Str(obj)) << endl;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <function>" << endl;
		return 1;
	}

	auto code = argv[1];

	// Initialize the Python Interpreter
	Py_Initialize();

	// Create python function from code
	cout << "Creating python function" << endl;
	ostringstream ss;
	ss << "lambda a,b: " << code << ends;
	auto codeString = ss.str();

	// Import the "math" module, and make all its functions available to the function
	auto pName = PyString_FromString("math");

	// Load the module object
	auto pModule = PyImport_Import(pName);
	auto pDict = PyModule_GetDict(pModule);

	auto func = PyRun_String(codeString.c_str(), Py_eval_input, pDict, PyDict_New());
	if (!func || !PyCallable_Check(func))
	{
		PyErr_Print();
		return -1;
	}

	// Call the funtion
	cout << "Calling the function for a=40, and b in [0,9]" << endl;

	auto arguments = PyTuple_New(2);
	PyTuple_SetItem(arguments, 0, PyInt_FromLong(40));
	for (size_t i = 0; i < 10; i++)
	{
		PyTuple_SetItem(arguments, 1, PyInt_FromLong(i));

		auto returned = PyObject_CallObject(func, arguments);
		cout << "Returned valued: " << PyFloat_AsDouble(returned) << endl;
	}

	// Clean up
	Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_DECREF(func);

	// Finish the Python Interpreter
	Py_Finalize();

	return 0;
}


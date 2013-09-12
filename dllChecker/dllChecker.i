/* File : example.i */
%module dllChecker

%{
#include "dllChecker.h"
%}

/* Let's just grab the original header file here */
%include "dllChecker.h"


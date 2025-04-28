#include <stdio.h>
#include <setjmp.h>
#include "sieve_exceptions.h"


ExceptionMode exceptionMode = EXCEPTION_MODE_C;

jmp_buf jumpBuffer;


void (*pythonExceptionHandler)(const char*, const char*) = NULL;


void raiseException(const char *exceptionMessage, const char *errorType) {
    if (exceptionMode == EXCEPTION_MODE_PYTHON && pythonExceptionHandler ) {
            pythonExceptionHandler(exceptionMessage, errorType);
            return;
    }else {
        fprintf(stderr, "%s: %s\n", errorType, exceptionMessage);
        longjmp(jumpBuffer, 1); // Jump back to the saved state
    }
}


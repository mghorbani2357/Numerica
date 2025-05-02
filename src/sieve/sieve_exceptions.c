#include <stdio.h>
#include <setjmp.h>
#include "sieve_exceptions.h"

jmp_buf jumpBuffer;

void raiseException(const char *exceptionMessage, const char *errorType) {
//    if (exceptionMode == EXCEPTION_MODE_PYTHON && pythonExceptionHandler ) {
//            pythonExceptionHandler(exceptionMessage, errorType);
//    }else {
        fprintf(stderr, "%s: %s\n", errorType, exceptionMessage);
        longjmp(jumpBuffer, 1); // Jump back to the saved state
//    }
}

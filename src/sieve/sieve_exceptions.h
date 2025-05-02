#ifndef SIEVE_EXCEPTIONS_H
#define SIEVE_EXCEPTIONS_H

//typedef enum {
//    EXCEPTION_MODE_C,
//    EXCEPTION_MODE_PYTHON
//} ExceptionMode;

void raiseException(const char *exceptionMessage, const char *errorType);
extern void pythonExceptionHandler(const char*, const char*);

#endif //SIEVE_EXCEPTIONS_H

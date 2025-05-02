#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

#include "sieve.h"
#include "sieve_exceptions.h"
#define SET_BIT(arr, i) (arr[(i) / 64] |= (1ULL << ((i) % 64)))
#define CLEAR_BIT(arr, i) (arr[(i) / 64] &= ~(1ULL << (63 -(i % 64))))
#define GET_BIT(arr, i) (arr[(i) / 64] & (1ULL << ((i) % 64)))
#define BIAS 1
//jmp_buf jumpBuffer;
//
//void raiseException(const char *exceptionMessage, const char *errorType) {
//    if (exceptionMode == EXCEPTION_MODE_PYTHON) {
//
//    }else {
//        fprintf(stderr, "%s: %s\n", errorType, exceptionMessage);
//        longjmp(jumpBuffer, 1); // Jump back to the saved state
//    }
//}

bool checkBit(UInt64Array array, uint64_t bitIndex) {
    if (array.size < 1)
        raiseException("`array` size is lower than `1`", "Invalid Array");

    uint64_t segmentIndex = bitIndex / 64;

    uint64_t shifts = 63 - (bitIndex % 64);

    return array.segments[segmentIndex] & (1ULL << shifts);
}


void clearBit(UInt64Array *array, uint64_t bitIndex) {
    if (array->size < 1ULL)
        raiseException("`array` size is lower than `1`", "Invalid Array");

    uint64_t segmentIndex = bitIndex / 64;

    uint64_t shifts = 63 - (bitIndex % 64);

    array->segments[segmentIndex] &= ~(1ULL << shifts);
}

uint64_t ceilDiv(uint64_t a, uint64_t b) {
    return (a + b - 1) / b;  // Safe ceil division for uint64_t
}

UInt64Array initializeUInt64Array(const uint64_t size) {

    UInt64Array unitArray;

    unitArray.segments = (uint64_t *) calloc(size, sizeof(uint64_t));

    unitArray.size = size;

    return unitArray;

}

SieveRange initializeSieveRange(const uint64_t start, const uint64_t end) {

    if (start >= end)
        raiseException("Exception happened during `initializeSieveArray`. "
                       "`end` variable must be bigger than `start`.", "Invalid input");

    uint64_t rangeSize = end - start;

    uint64_t segmentCount = ceilDiv(rangeSize, 64);

    UInt64Array array = initializeUInt64Array(segmentCount);

    for (uint64_t i = 0; i < array.size; i++)
        array.segments[i] = -1;

    SieveRange range;

    range.array = array;
    range.start = start;
    range.end = end;
    range.rangeSize = rangeSize;

    return range;
}

bool fileExists(const char *path) {

    struct stat buffer;

    return (stat(path, &buffer) == 0);
}

uint64_t getFileSize(const char *path) {

    if (!fileExists(path))
        raiseException("File with given path does not found", "File Not Found");

    struct stat st;

    if (stat(path, &st) == 0)
        return st.st_size; // File size from metadata

    return -1;
}

void saveSieveRangeIntoFile(const SieveRange *sieveRange, const char *path) {
    FILE *file = fopen(path, "wb"); // Open in binary write mode
    if (!file)
        raiseException("Failed to open the file", "File Error");

    fwrite(&sieveRange->start, sizeof(uint64_t), 1, file);
    fwrite(&sieveRange->end, sizeof(uint64_t), 1, file);
    fwrite(&sieveRange->rangeSize, sizeof(uint64_t), 1, file);
    fwrite(&sieveRange->array.size, sizeof(uint64_t), 1, file);

    fwrite(sieveRange->array.segments, sizeof(uint64_t), sieveRange->array.size, file);

    fclose(file);
}

SieveRange *loadSieveRangeFromFile(const char *path) {
    if (!fileExists(path))
        raiseException("File with given path does not found", "File Not Found");

    SieveRange *range = malloc(sizeof(SieveRange));

    if (!range)
        raiseException("Memory allocation failed", "Memory Allocation Error");

    FILE *file = fopen(path, "rb");

    if (!file)
        raiseException("Failed to open the file", "File Error");

    fread(&range->start, sizeof(uint64_t), 1, file);
    fread(&range->end, sizeof(uint64_t), 1, file);
    fread(&range->rangeSize, sizeof(uint64_t), 1, file);
    fread(&range->array.size, sizeof(uint64_t), 1, file);

    range->array.segments = malloc(range->array.size * sizeof(uint64_t));

    if (!range->array.segments) {
        free(range);
        fclose(file);
        raiseException("Memory allocation for array failed", "Memory Allocation Error");
    }

    fread(range->array.segments, sizeof(uint64_t), range->array.size, file);

    fclose(file);

    return range;
}

SieveRange extractPrimes(const uint64_t start, const u_int64_t end, const SieveRange necessaryPrimes) {

    SieveRange candidatePrimes = initializeSieveRange(start, end);

    uint64_t limit = (uint64_t) ceil(sqrt((double) end)); // Note: This Work until 2^64

    uint64_t loopBound = limit - necessaryPrimes.start;

    for (uint64_t i = 0; i < loopBound; i++) {

        int j = 0;
        if (checkBit(necessaryPrimes.array, i)) {
            uint64_t prime = i + necessaryPrimes.start;
            uint64_t shift = (!(candidatePrimes.start % prime)) ? 0 : prime - (candidatePrimes.start % prime);

            while (j * prime + shift < necessaryPrimes.rangeSize)
                clearBit(&candidatePrimes.array, shift + prime * j++);
        }
    }
    return candidatePrimes;
}

SieveRange extendSieveRanges(const SieveRange alpha, const SieveRange beta) {

    if (alpha.end != beta.start)
        raiseException("Invalid range to extend", "Extension Error");

    SieveRange extended;
    extended.start = alpha.start;
    extended.end = beta.end;
    extended.rangeSize = extended.end - extended.start;

    uint64_t segmentCount = alpha.array.size + beta.array.size;

    UInt64Array array = initializeUInt64Array(segmentCount);

    uint64_t i = 0, j = 0;

    for (i = 0; i < alpha.array.size; i++)
        array.segments[i] = alpha.array.segments[i];

    for (j = 0; j < beta.array.size; j++, i++)
        array.segments[i] = beta.array.segments[j];

    extended.array = array;

    return extended;
}

void printSieveRange(SieveRange range) {

    for (uint64_t i = 0; i < range.rangeSize; i++) {
        if (checkBit(range.array, i))
            printf("%llu\n", range.start + i);
    }

}
void freeSieveRange(SieveRange *range){
    if (!range) return;

    if (range->array.segments) {
        free(range->array.segments);
    }

    free(range);  // Frees the SieveRange struct itself
}
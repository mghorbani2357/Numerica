#ifndef SIEVE_H
#define SIEVE_H

typedef struct {
    uint64_t *segments;
    uint64_t size;
} UInt64Array;

typedef struct {
    UInt64Array array;
    uint64_t start;
    uint64_t end;
    uint64_t rangeSize;
} SieveRange;

UInt64Array initializeUInt64Array(uint64_t size);

SieveRange initializeSieveRange(uint64_t start, uint64_t end);

void saveSieveRangeIntoFile(const SieveRange *sieveRange, const char *path);

SieveRange *loadSieveRangeFromFile(const char *path);

SieveRange extractPrimes(uint64_t start, u_int64_t end, SieveRange necessaryPrimes);

SieveRange extendSieveRanges(SieveRange alpha, SieveRange beta);

void printSieveRange(SieveRange range);

void freeSieveRange(SieveRange *range);

#endif //SIEVE_H

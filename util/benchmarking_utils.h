#ifndef SYSPROG_BENCHMARKING_UTILS_H
#define SYSPROG_BENCHMARKING_UTILS_H

#include <stdint.h>

typedef struct {
    char function_name[50];
    uint64_t arguments[8];
    uint8_t no_args;
} BenchReading;

#ifdef BENCHMARK
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define BENCH_TRACE(fn, ...) do { \
    BenchReading reading = { .function_name = fn,  .arguments = {__VA_ARGS__}, .no_args = 0 }; \
    if (NUMARGS(__VA_ARGS__) == 0) reading.no_args = 1; \
    append_trace_binary_file(&reading); \
    } while(0)
#else
#define BENCH_TRACE(fn, ...) do {} while(0)
#endif

#include <stdio.h>
#include <assert.h>


void generate_file_name();
void append_trace_binary_file(BenchReading*);

#endif //SYSPROG_BENCHMARKING_UTILS_H
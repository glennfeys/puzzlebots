#include "benchmarking_utils.h"
#include <time.h>
#include <stdio.h>

char name[100];

void generate_file_name(){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(name, sizeof(name)-1, "./benchmarks/BenchMark %d-%m-%Y %Hu%M", t);
    printf("Current Date: %s\n", name);
}

void append_trace_binary_file(BenchReading* reading){
    FILE * file= fopen(name, "ab");
    if (file != NULL) {
        fwrite(&reading->function_name, 50, 1, file);
        fwrite(&reading->arguments, 64, 1, file);
        fwrite(&reading->no_args, 1, 1, file);
        fclose(file);
    }
}

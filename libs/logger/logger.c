// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
static FILE* logger_fp = NULL;
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: COLORING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void logger_print_stdout_header(logger_level level) {
    // Print logging level
    if (level == LOGGER_INFO)
        printf(LOGGER_BLUE"[INFO]:"LOGGER_RESET" ");
    else if (level == LOGGER_WARN)
        printf(LOGGER_YELLOW"[WARN]:"LOGGER_RESET" ");
    else if (level == LOGGER_ERROR)
        printf(LOGGER_RED"[ERRO]:"LOGGER_RESET" ");
    else if (level == LOGGER_SUCC)
        printf(LOGGER_GREEN"[SUCC]:"LOGGER_RESET" ");
    
    return;
}

void logger_print_log_file_header(logger_level level) {
    if (logger_fp == NULL)
        return;
    
    // Print timestamp
    time_t now   = time(NULL);
    struct tm* t = localtime(&now);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long milis = ts.tv_nsec / 1000000;

    fprintf(logger_fp, "[%4d-%2d-%2d %2d:%2d:%2d.%3ld] ", t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, milis);

    // Print logging level
    if (level == LOGGER_INFO)
        fprintf(logger_fp, "INFO: ");
    else if (level == LOGGER_WARN)
        fprintf(logger_fp, "WARN: ");
    else if (level == LOGGER_ERROR)
        fprintf(logger_fp, "ERROR: ");
    else if (level == LOGGER_SUCC)
        fprintf(logger_fp, "SUCC: ");

    return;
}

void logger_log(logger_level level, const char* message, ...) {
    va_list args;

    // Print header
    logger_print_stdout_header(level);
    
    // Write to stdout
    va_start(args, message);
    vfprintf(stdout, message, args);
    va_end(args);

    if (logger_fp == NULL) return;

    // Print timestamped header
    logger_print_log_file_header(level);

    // Write to log file
    va_start(args, message);
    vfprintf(logger_fp, message, args);
    va_end(args);
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int logger_init(const char *file_path) {
    if (logger_fp == NULL)
        logger_fp = fopen(file_path, "w");

    if (logger_fp == NULL) {
        logger_log(LOGGER_WARN, "Could not open log file %s: %s\n", file_path, strerror(errno));
        return 1;
    }
        
    return 0;
}

void logger_close() {
    if (logger_fp != NULL) {
        fflush(logger_fp);
        fclose(logger_fp);
        logger_fp = NULL;
    }
    return;    
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
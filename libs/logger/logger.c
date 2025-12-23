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





// :::: UTILS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int logger_generate_timestamp(char* buffer, int capacity, const char* fmt) {
    time_t now   = time(NULL);
    struct tm* t = localtime(&now);

    int needed = strlen(fmt);
    if (needed > capacity) {
        fprintf(stderr, "Capacity of %d is insufficient, needed: %d\n", capacity, needed);
        return 1;
    }

    snprintf(buffer, capacity, fmt, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return 0;
}
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

    fprintf(logger_fp, "[%04d-%02d-%02d %02d:%02d:%02d.%03ld] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, milis);

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

int logger_add_timestamp_to_filepath(char* file_path, int capacity) {
    // Find dot indicating the extension
    char* last_dot = strrchr(file_path, '.');
    int dot_idx    = (int)(last_dot - file_path);

    // Separate string into two parts
    char left_part[capacity], right_part[capacity];
    strncpy(left_part, file_path, dot_idx);
    left_part[dot_idx] = '\0';
    strncpy(right_part, file_path + dot_idx + 1, strlen(file_path) - dot_idx);
    right_part[dot_idx] = '\0';

    // Generate timestamp
    char timestamp[35];
    logger_generate_timestamp(timestamp, sizeof(timestamp), "%04d-%02d-%02d_____%02d:%02d:%02d");

    int needed = strlen(left_part) + strlen(timestamp) + strlen(right_part);
    if (needed > capacity) {
        fprintf(stderr, "Capacity of %d is insuficient: needed %d\n", capacity, needed);
        return 1;
    }

    // Add timestamp before last dots
    sprintf(file_path, "%s_%s.%s", left_part, timestamp, right_part);
    return 0;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int logger_init(const char *file_path) {
    if (logger_fp == NULL)
        logger_fp = fopen(file_path, "w");

    if (logger_fp == NULL) {
        fprintf(stderr, "Could not open log file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

    return 0;
}

void logger_close(void) {
    if (logger_fp != NULL) {
        fflush(logger_fp);
        fclose(logger_fp);
        logger_fp = NULL;
    }
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

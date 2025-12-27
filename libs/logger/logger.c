// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define LOGGER_RED    "\x1b[31m" /** Initializes RED color.    */
#define LOGGER_GREEN  "\x1b[32m" /** Initializes GREEN color.  */
#define LOGGER_YELLOW "\x1b[33m" /** Initializes YELLOW color. */
#define LOGGER_BLUE   "\x1b[34m" /** Initializes BLUE color.   */
#define LOGGER_RESET   "\x1b[0m" /** Restores normal color.    */

/**
 * @brief Global pointer to the log file.
 */
static FILE* log_file_ptr = NULL;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: HELPERS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Print a colored header based on the log level.
 *
 * @param level The logging level to print the header for
 */
static void print_stdout_header(logger_level_t level) {
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

/**
 * @brief Print a timestamped header for the message written in the log file.
 *
 * @param level The logging level to print the header for
 */
static void print_log_file_header(logger_level_t level) {
    if (log_file_ptr == NULL)
        return;

    // Generate timestamp
    time_t now   = time(NULL);
    struct tm* t = localtime(&now);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long milis = ts.tv_nsec / 1000000;

    fprintf(log_file_ptr, "[%04d-%02d-%02d %02d:%02d:%02d.%03ld] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, milis);

    // Print logging level
    if (level == LOGGER_INFO)
        fprintf(log_file_ptr, "INFO: ");
    else if (level == LOGGER_WARN)
        fprintf(log_file_ptr, "WARN: ");
    else if (level == LOGGER_ERROR)
        fprintf(log_file_ptr, "ERROR: ");
    else if (level == LOGGER_SUCC)
        fprintf(log_file_ptr, "SUCC: ");

    return;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: UTILS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int logger_add_timestamp_to_filepath(char* file_path, int capacity) {
    // Find dot indicating the extension
    int last_dot_idx = 0;
    int str_len = strlen(file_path);

    for (int i = 0; i < str_len; i++) {
        if (file_path[i] == '.') last_dot_idx = i;
    }

    // Separate string into two parts
    char left_part[capacity], right_part[capacity];
    strncpy(left_part, file_path, last_dot_idx);
    left_part[last_dot_idx] = '\0';
    strncpy(right_part, file_path + last_dot_idx + 1, capacity - last_dot_idx);
    right_part[last_dot_idx] = '\0';

    // Generate timestamp
    char timestamp[128];

    time_t now   = time(NULL);
    struct tm* t = localtime(&now);

    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d_%02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    int needed = strlen(left_part) + strlen(timestamp) + strlen(right_part);
    if (needed > capacity) {
        logger_log(LOGGER_ERROR, "Capacity of %d is insuficient: needed %d\n", capacity, needed);
        return 1;
    }

    // Add timestamp before last dots
    sprintf(file_path, "%s_%s.%s", left_part, timestamp, right_part);
    return 0;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOGGING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void logger_log(logger_level_t level, const char* message, ...) {
    va_list args;

    // Print header
    print_stdout_header(level);

    // Write to stdout
    va_start(args, message);
    vfprintf(stdout, message, args);
    va_end(args);

    if (log_file_ptr == NULL) return;

    // Print timestamped header
    print_log_file_header(level);

    // Write to log file
    va_start(args, message);
    vfprintf(log_file_ptr, message, args);
    va_end(args);
    return;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int logger_init(const char *file_path) {
    if (log_file_ptr == NULL)
        log_file_ptr = fopen(file_path, "w");

    if (log_file_ptr == NULL) {
        logger_log(LOGGER_ERROR, "Could not open log file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

    return 0;
}

void logger_close(void) {
    if (log_file_ptr != NULL) {
        fflush(log_file_ptr);
        fclose(log_file_ptr);
        log_file_ptr = NULL;
    }
    return;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

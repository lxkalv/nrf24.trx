// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>
#include <errno.h>

#include "logger.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



// :::: COLORING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#define LOGGER_COLOR_RED    "\x1b[31m"
#define LOGGER_COLOR_GREEN  "\x1b[32m"
#define LOGGER_COLOR_YELLOW "\x1b[33m"
#define LOGGER_COLOR_BLUE   "\x1b[34m"
#define LOGGER_COLOR_RESET   "\x1b[0m"

void get_colored_header(logger_level level) {
    if (level == LOGGER_LEVEL_INFO)
        return;
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
static FILE* logger_fp = NULL;

int logger_init(const char *file_path) {
    if (logger_fp == NULL)
        logger_fp = fopen(file_path, "w");

    if (logger_fp == NULL)
        fprintf(stderr, "Could not open log file %s: %s\n",
                file_path, strerror(errno));

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
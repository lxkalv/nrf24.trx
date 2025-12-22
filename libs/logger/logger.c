// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "logger.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: COLORING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
char* logger_get_colored_header(logger_level level) {
    if (level == LOGGER_INFO)
        return LOGGER_BLUE"[INFO]:"LOGGER_RESET;
    else if (level == LOGGER_WARN)
        return LOGGER_YELLOW"[WARN]:"LOGGER_RESET;
    else if (level == LOGGER_ERROR)
        return LOGGER_RED"[ERRO]:"LOGGER_RESET;
    else if (level == LOGGER_SUCC)
        return LOGGER_GREEN"[SUCC]:"LOGGER_RESET;
    else
        return "";
}

// void logger_log(logger_level level, char* message, const char* fmt, ...) {
void logger_log(logger_level level, char* message) {
    // va_list args;
    // va_start(args, fmt);
    char* header = logger_get_colored_header(level);
    // char buffer[128];
    printf("%s %s", header, message);
    // va_end(args);
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
static FILE* logger_fp = NULL;

int logger_init(const char *file_path) {
    if (logger_fp == NULL)
        logger_fp = fopen(file_path, "w");

    if (logger_fp == NULL) {
        fprintf(stderr, "Could not open log file %s: %s\n", file_path, strerror(errno));
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
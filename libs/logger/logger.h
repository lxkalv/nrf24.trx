#ifndef   LOGGER_H
#define   LOGGER_H

typedef enum {
    LOGGER_LEVEL_INFO = 0,
    LOGGER_LEVEL_WARN,
    LOGGER_LEVEL_ERROR,
    LOGGEL_LEVEL_SUCC,
} logger_level;

// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @brief Initialize the logger.
 * @param file_path Path to the log file
*/
int logger_init(const char *file_path);
void logger_close(void);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // LOGGER_H
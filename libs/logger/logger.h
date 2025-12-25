#ifndef   LOGGER_H
#define   LOGGER_H

// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Enum representing the log level of the message.
 */
typedef enum {
    LOGGER_INFO = 0,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_SUCC,
} logger_level;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: UTILS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Add a timestamp to the provided file path.
 *
 * @param file_path The file_path to add the timestamp to.
 */
int logger_add_timestamp_to_filepath(char* file_path, int capacity);

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOGGING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Print a colored message to stdout and log file.
 *
 * Print a message to stdout with a colored header based on the log level. If the
 * logger is initialized with a log file. It will also log the line to the file
 * with a timestamp and without the ANSII escape sequences.
 *
 * @param level The logging level to be used
 * @param message The string to be logged
 */
void logger_log(logger_level level, const char* message, ...);

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LOG FILE HANDLE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Initialize the logger.
 *
 * Create a log file where the logger will log to. This function can safely be
 * called multiple times.
 *
 * @param file_path Path to the log file.
 */
int logger_init(const char* file_path);

/**
 * @brief Close the logger.
 *
 * Closes and flushed the log file. This function can safely be called multiple
 * times.
 */
void logger_close(void);

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // LOGGER_H

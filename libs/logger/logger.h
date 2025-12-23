#ifndef   LOGGER_H
#define   LOGGER_H

// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#define LOGGER_RED    "\x1b[31m"
#define LOGGER_GREEN  "\x1b[32m"
#define LOGGER_YELLOW "\x1b[33m"
#define LOGGER_BLUE   "\x1b[34m"
#define LOGGER_RESET   "\x1b[0m"

typedef enum {
    LOGGER_INFO = 0,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_SUCC,
} logger_level;
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: UTILS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @brief Fill the provided buffer with a timestamp.
 *
 * The format NEEDS to follow the following order:
 * - YEAR
 * - MONTH
 * - DAY
 * - HOUR
 * - MINUTE
 * - SECOND
 *
 * Example format: YYYY-MM-DD_HH:MM:SS -> %04d-%02d-%02d_%02d:%02d:%02d
 *
 * @param buffer The buffer that will be filled up with the timestamp.
 * @param capacity The size of the buffer.
 * @param fmt The string format to follow.
 */
int logger_generate_timestamp(char* buffer, int capacity, const char* fmt);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: COLORING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @brief Print a colored header based on the log level.
 *
 * @param level The logging level to print the header for
 */
void logger_print_stdout_header(logger_level level);

/**
 * @brief Print a timestamped header for the message written in the log file.
 *
 * @param level The logging level to print the header for
 */
void logger_print_log_file_header(logger_level level);

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

/**
 * @brief Add a timestamp to the provided file path.
 *
 * @param file_path The file_path to add the timestamp to.
 */
int logger_add_timestamp_to_filepath(char* file_path, int capacity);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // LOGGER_H

#include <linux/limits.h>
#include <unistd.h>

#include "logger/logger.h"

int main(void) {
    char log_path[PATH_MAX] = "logs/tests/logger/test_init.log.log.log";
    logger_log(LOGGER_INFO, "Initializing logger on path: %s. This message will be out of the log\n", log_path);

    if (logger_init(log_path) != 0) return 1;

    logger_log(LOGGER_INFO,  "This info message will be on the file\n"); usleep(10 * 1000);
    logger_log(LOGGER_INFO,  "This info message will be on the file\n"); usleep(10 * 1000);
    logger_log(LOGGER_INFO,  "This info message will be on the file\n"); usleep(10 * 1000);

    logger_close();

    logger_log(LOGGER_WARN,  "This warning message will NOT be on the file\n"); usleep(10 * 1000);



    logger_add_timestamp_to_filepath(log_path, sizeof(log_path));
    logger_log(LOGGER_INFO, "Adding timestamp to file name, now %s\n", log_path);

    if (logger_init(log_path) != 0) return 1;

    logger_log(LOGGER_INFO,  "This info message will be on the timestamped file\n"); usleep(10 * 1000);
    logger_log(LOGGER_INFO,  "This info message will be on the timestamped file\n"); usleep(10 * 1000);
    logger_log(LOGGER_INFO,  "This info message will be on the timestamped file\n"); usleep(10 * 1000);

    logger_close();
    return 0;
}

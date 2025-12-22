#include <stdio.h>
#include <unistd.h>

#include "logger/logger.h"

int main(void) {
    char* log_path = "logs/test.log";
    if (logger_init(log_path) == 0)
        logger_log(LOGGER_INFO, "Logging to: %s\n", log_path);

    logger_log(LOGGER_INFO,  "This is a test for an INFO logging message\n");
    usleep(10 * 1000);
    logger_log(LOGGER_WARN,  "This is a test for an WARN logging message\n");
    usleep(10 * 1000);
    logger_log(LOGGER_ERROR, "This is a test for an ERROR logging message\n");
    usleep(10 * 1000);
    logger_log(LOGGER_SUCC,  "This is a test for an SUCC logging message\n");

    logger_close();
    return 0;
}
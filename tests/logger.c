#include <stdio.h>

#include "logger/logger.h"

int main() {
    char* log_path = "logs/test.log";

    if (logger_init(log_path) != 0)
        logger_log(LOGGER_ERROR, "An error ocurred!");
    else
        logger_log(LOGGER_INFO, "Logging to file\n");


    logger_close();
    return 0;
}
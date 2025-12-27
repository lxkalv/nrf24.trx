#include "logger/logger.h"

int main(void) {
    logger_log(LOGGER_INFO, "%d: This is a formated info message\n", 0);
    logger_log(LOGGER_WARN, "%d: This is a formated warning message\n", 1);
    logger_log(LOGGER_ERROR, "%d: This is a formated error message\n", 2);
    logger_log(LOGGER_SUCC, "%d: This is a formated success message\n", 3);
    return 0;
}

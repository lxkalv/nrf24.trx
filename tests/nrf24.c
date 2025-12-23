#include "logger/logger.h"
#include "nrf24/nrf24.h"

int main() {
    char log_file_path[128] = "logs/nrf24.log";
    logger_add_timestamp_to_filepath(log_file_path, sizeof(log_file_path));

    if (logger_init(log_file_path) != 0) {
        logger_log(LOGGER_WARN, "Could not initialize log path: %s\n", log_file_path);
    }

    nrf24_print_user_radio_config();

    logger_close();
    return 0;
}

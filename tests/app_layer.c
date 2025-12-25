#include <stdio.h>

#include "app_layer/app_layer.h"
#include "nrf24/nrf24.h"
#include "logger/logger.h"

int main(int argc, char** argv) {
    char log_file_path[128] = "logs/app_layer.log";
    if (logger_init(log_file_path) != 0) logger_log(LOGGER_WARN, "Could not initialize log file %s\n", log_file_path);

    if (app_parse_user_config(argc, argv) != 0) return 1;

    app_print_app_config();
    printf("\n");
    nrf24_print_user_radio_config();
    logger_close();
    return 0;
}

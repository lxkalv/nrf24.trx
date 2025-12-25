// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>

#include "logger/logger.h"
#include "nrf24/nrf24.h"

#include "app_layer.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Global app configuration.
 */
app_config_t app_config = {
    .mode      = APP_MODE_UNSET,
    .protocol  = APP_PROTOCOL_UNSET,
    .file_path = {0},
};

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: HELPERS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Print the app usage and additional flags.
 *
 * @param program_name The name of the program for displaying
 */
static void print_usage(const char* program_name) {
    logger_log(LOGGER_INFO,
        "NRF24.TRX USAGE:\n"
        "    %s --mode {TX, RX} --protocol {P2P, NETWORK} [options]\n"
        "\n"
        "APP:\n"
        "    --mode                       {TX, RX} (REQUIRED) Role of this node in the transmission (case insensitive)\n"
        "    --protocol             {P2P, NETWORK} (REQUIRED) Protocol to use for the transmission (case insensitive)\n"
        "    --file-path                    [PATH] File path to transmit or receive. If not provided, will look for suitable file in USB\n"
        "\n"
        "RADIO:\n"
        "    --spi-speed                   {1..10} (default = 10) The speed at which the MCU interacts with the RF module in MHz\n"
        "    --ce-pin                    {idk yet} (default = 22) The GPIO pin of the MCU selected to interact with the RF module\n"
        "    --channel                    {0..125} (default = 76) Channel to use for the transmission (only applies to P2P protocol)\n"
        "    --data-rate   {250KBPS, 1MBPS, 2MBPS} (default = 1MBPS) Data rate to use for the transmission (case insensitive)\n"
        "    --pa-level      {MIN, LOW, HIGH, MAX} (default = MIN) Output power of the NRF24 chip (case insensitive)\n"
        "    --crc-length                   {0..2} (default = 2) Number of bytes to use in the CRC of each frame\n"
        "    --rtx-retries                 {0..15} (default = 15) Number of retries for each frame\n"
        "    --rtx-delay                   {0..15} (default = 2) Delay between retries of the same frame\n",
        program_name
    );

    return;
}

/**
 * @brief Return a human-readable string based on the app mode value.
 */
static const char* get_mode(void) {
    switch (app_config.mode) {
        case APP_MODE_RX: return        "RX";
        case APP_MODE_TX: return        "TX";
        default:          return "UNDEFINED";
    }
}

/**
 * @brief Return a human-readable string based on the app protocol value.
 */
static const char* get_protocol(void) {
    switch (app_config.protocol) {
        case APP_PROTOCOL_P2P:     return       "P2P";
        case APP_PROTOCOL_NETWORK: return   "NETWORK";
        default:                   return "UNDEFINED";
    }
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: ARG PARSING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int app_parse_user_config(int argc, char **argv) {
    int mode_has_been_set     = 0;
    int protocol_has_been_set = 0;

    int idx = 1; // NOTE: skip first argument as it is the program name
    while (idx < argc) {
        if (strcmp(argv[idx], "--help") == 0) {
            print_usage(argv[0]);
            return 1;
        }

        else if (strcmp(argv[idx], "--mode") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --mode flag. Must be one of {TX, RX} (case insensitive)\n");
                return 1;
            }

            mode_has_been_set = 1;
            if (strcasecmp(argv[idx], "tx") == 0) {
                app_config.mode = APP_MODE_TX;
            } else if (strcasecmp(argv[idx], "rx") == 0) {
                app_config.mode = APP_MODE_RX;
            } else {
                logger_log(LOGGER_ERROR, "Invalid value for --mode flag %s. Must be one of {TX, RX} (case insensitive)\n", argv[idx]);
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--protocol") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --protocol flag. Must be one of {P2P, NETWORK} (case insensitive)\n");
                return 1;
            }

            protocol_has_been_set = 1;
            if (strcasecmp(argv[idx], "p2p") == 0) {
                app_config.protocol = APP_PROTOCOL_P2P;
            } else if (strcasecmp(argv[idx], "network") == 0) {
                app_config.protocol = APP_PROTOCOL_NETWORK;
            } else {
                logger_log(LOGGER_ERROR, "Invalid value for --protocol flag %s. Must be one of {P2P, NETWORK} (case insensitive)\n", argv[idx]);
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--file-path") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --file-path flag\n");
                return 1;
            }

            if (strlen(argv[idx]) > sizeof(app_config.file_path)) {
                logger_log(LOGGER_ERROR, "Not enough memory allocated for file path. Required %d. Consider increasing the buffer size\n", strlen(argv[idx]));
                return 1;
            }

            strncpy(app_config.file_path, argv[idx], sizeof(app_config.file_path));
        }

        else if (strcmp(argv[idx], "--spi-speed") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --spi-speed\n");
                return 1;
            }

            char* endptr = argv[idx];
            errno = 0;
            int spi_speed = (int)strtol(argv[idx], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0') {
                logger_log(LOGGER_ERROR, "Cound not convert spi speed %s to int\n", argv[idx]);
                return 1;
            }

            if (nrf24_set_spi_speed(spi_speed) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --spi-speed flag. Must be one of {0..10}\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--ce-pin") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --ce-pin flag. Must be one of {idk yet}\n");
                return 1;
            }

            char* endptr = argv[idx];
            errno = 0;
            int ce_pin = (int)strtol(argv[idx], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0') {
                logger_log(LOGGER_ERROR, "Could not convert ce pin %s to int\n", argv[idx]);
                return 1;
            }

            if (nrf24_set_ce_pin(ce_pin) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --ce-pin flag. Must be one of {idk yet}\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--channel") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --channel flag. Must be one of {0..125}\n");
                return 1;
            }

            char* endptr = argv[idx];
            errno = 0;
            int channel = (int)strtol(argv[idx], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0') {
                logger_log(LOGGER_ERROR, "Could not convert channel %s to int\n", argv[idx]);
                return 1;
            }

            if (nrf24_set_channel(channel) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --channel flag. Must be one of {0..125}\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--data-rate") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --data-rate flag. Must be one of {250KBPS, 1MBPS, 2MBPS} (case insensitive)\n");
                return 1;
            }

            nrf24_data_rate data_rate;
            if (strcasecmp(argv[idx], "250kbps") == 0) {
                data_rate = NRF24_DATA_RATE_250KBPS;
            } else if (strcasecmp(argv[idx], "1mbps") == 0) {
                data_rate = NRF24_DATA_RATE_1MBPS;
            } else if (strcasecmp(argv[idx], "2mbps") == 0) {
                data_rate = NRF24_DATA_RATE_2MBPS;
            } else {
                data_rate = NRF24_DATA_RATE_UNSET;
            }

            if (nrf24_set_data_rate(data_rate) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --data-rate flag. Must be one of {250KBPS, 1MBPS, 2MBPS} (case insensitive\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--pa-level") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --pa-level flag. Must be one of {MIN, LOW, HIGH, MAX} (case insensitive)\n");
                return 1;
            }

            nrf24_pa_level pa_level;
            if (strcasecmp(argv[idx], "min") == 0) {
                pa_level = NRF24_PA_LEVEL_MIN;
            } else if (strcasecmp(argv[idx], "low") == 0) {
                pa_level = NRF24_PA_LEVEL_LOW;
            } else if (strcasecmp(argv[idx], "high") == 0) {
                pa_level = NRF24_PA_LEVEL_HIGH;
            } else if (strcasecmp(argv[idx], "max") == 0) {
                pa_level = NRF24_PA_LEVEL_MAX;
            } else {
                pa_level = NRF24_PA_LEVEL_UNSET;
            }

            if (nrf24_set_pa_level(pa_level) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --pa-level flag. Must be one of {MIN, LOW, HIGH, MAX} (case insensitive)\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--crc-length") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --crc-length flag. Must be one of {0..2}\n");
                return 1;
            };

            nrf24_crc_length crc_length;
            if (strcmp(argv[idx], "0") == 0) {
                crc_length = NRF24_CRC_NONE;
            } else if (strcmp(argv[idx], "1") == 0) {
                crc_length = NRF24_CRC_1_BYTES;
            } else if (strcmp(argv[idx], "2") == 0) {
                crc_length = NRF24_CRC_2_BYTES;
            } else {
                crc_length = NRF24_CRC_UNSET;
            }

            if (nrf24_set_crc_length(crc_length) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --crc-length flag. Must be one of {0..2}\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--rtx-retries") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --rtx-retries flag. Must be one of {0..15}\n");
                return 1;
            }

            char* endptr = argv[idx];
            errno = 0;
            int rtx_retries = (int)strtol(argv[idx], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0') {
                logger_log(LOGGER_ERROR, "Could not convert rtx retries %s to int\n", argv[idx]);
                return 1;
            }

            if (nrf24_set_rtx_retries(rtx_retries) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --rtx-retries flag. Must be one of {0..15}\n");
                return 1;
            }
        }

        else if (strcmp(argv[idx], "--rtx-delay") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --rtx-delay flag. Must be one of {0..15}\n");
                return 1;
            }

            char* endptr = argv[idx];
            errno = 0;
            int rtx_delay = (int)strtol(argv[idx], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0') {
                logger_log(LOGGER_ERROR, "Could not convert rtx delay %s to int\n", argv[idx]);
                return 1;
            }

            if (nrf24_set_rtx_delay(rtx_delay) != 0) {
                logger_log(LOGGER_ERROR, "Invalid value for --rtx-delay flag. Must be one of {0..15}\n");
                return 1;
            }
        }

        idx++;
    }

    if (!mode_has_been_set || !protocol_has_been_set) {
        logger_log(LOGGER_ERROR,
            "Required arguments missing\n"
            "    MODE     : %s\n"
            "    PROTOCOL : %s\n",
            mode_has_been_set     ? "PROVIDED" : "NOT PROVIDED",
            protocol_has_been_set ? "PROVIDED" : "NOT PROVIDED"
        );
        printf("\n");
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}

void app_print_app_config(void) {
    logger_log(LOGGER_INFO,
        "USER-DEFINED APP CONFIG:\n"
        "    %-11s : %14s\n"  // MODE
        "    %-11s : %14s\n"  // PROTOCOL
        "    %-11s : %14s\n", // FILE PATH
        "Mode",     get_mode(),
        "Protocol", get_protocol(),
        "File path", strlen(app_config.file_path) == 0 ? "<USB>" : app_config.file_path
    );
    return;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LED CONTROL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TODO: ...
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

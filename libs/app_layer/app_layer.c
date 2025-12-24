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
app_config user_config = {
    .mode       = APP_MODE_UNSET,
    .protocol   = APP_PROTOCOL_UNSET,
};
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// :::: HELPERS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
const char* app_get_mode(void) {
    switch (user_config.mode) {
        case APP_MODE_RX: return        "RX";
        case APP_MODE_TX: return        "TX";
        default:          return "UNDEFINED";
    }
}

const char* app_get_protocol(void) {
    switch (user_config.protocol) {
        case APP_PROTOCOL_P2P:     return       "P2P";
        case APP_PROTOCOL_NETWORK: return   "NETWORK";
        default:                   return "UNDEFINED";
    }
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// :::: ARG PARSING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void app_print_usage(const char* program_name) {
    logger_log(LOGGER_INFO,
        "NRF24.TRX USAGE:\n"
        "    %s --mode {TX, RX} --protocol {P2P, NETWORK} [options]\n"
        "\n"
        "REQUIRED:\n"
        "    --mode                       {TX, RX} Role of this node in the transmission (case insensitive)\n"
        "    --protocol             {P2P, NETWORK} Protocol to use for the transmission (case insensitive)\n"
        "\n"
        "OPTIONS:\n"
        "    --channel                    {0..125} Channel to use for the transmission (only applies to P2P protocol)\n"
        "    --data-rate   {250KBPS, 1MBPS, 2MBPS} Data rate to use for the transmission (case insensitive)\n"
        "    --pa-level      {MIN, LOW, HIGH, MAX} Output power of the NRF24 chip (case insensitive)\n"
        "    --crc-length                   {0..2} Number of bytes to use in the CRC of each frame\n"
        "    --rtx-retries                 {0..15} Number of retries for each frame\n"
        "    --rtx-delay                   {0..15} Delay between retries of the same frame\n",
        program_name
    );

    return;
}

int app_read_user_config_from_args(int argc, char **argv) {
    int mode_has_been_set     = 0;
    int protocol_has_been_set = 0;

    int idx = 1; // NOTE: skip first argument as it is the program name
    while (idx < argc) {
        if (strcmp(argv[idx], "--help") == 0) {
            app_print_usage(argv[0]);
            return 1;
        }

        else if (strcmp(argv[idx], "--mode") == 0) {
            if ((++idx + 1) > argc) {
                logger_log(LOGGER_ERROR, "Missing value for --mode flag. Must be one of {TX, RX} (case insensitive)\n");
                return 1;
            }

            mode_has_been_set = 1;
            if (strcasecmp(argv[idx], "tx") == 0) {
                user_config.mode = APP_MODE_TX;
            } else if (strcasecmp(argv[idx], "rx") == 0) {
                user_config.mode = APP_MODE_RX;
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
                user_config.protocol = APP_PROTOCOL_P2P;
            } else if (strcasecmp(argv[idx], "network") == 0) {
                user_config.protocol = APP_PROTOCOL_NETWORK;
            } else {
                logger_log(LOGGER_ERROR, "Invalid value for --protocol flag %s. Must be one of {P2P, NETWORK} (case insensitive)\n", argv[idx]);
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
                fprintf(stderr, "Could not convert channel %s to int\n", argv[idx]);
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
                fprintf(stderr, "Could not convert rtx retries %s to int\n", argv[idx]);
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
                fprintf(stderr, "Could not convert rtx delay %s to int\n", argv[idx]);
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
        logger_log(LOGGER_ERROR, "Required arguments missing\n");
        app_print_usage(argv[0]);
        return 1;
    }

    return 0;
}

void app_print_user_config(void) {
    logger_log(LOGGER_INFO,
        "USER-DEFINED APP DETAILS:\n"
        "    %-8s : %9s\n"  // MODE
        "    %-8s : %9s\n", // PROTOCOL
        "Mode",     app_get_mode(),
        "Protocol", app_get_protocol()
    );
    return;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

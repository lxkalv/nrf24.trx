// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>

#include "logger/logger.h"

#include "nrf24.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
nrf24_config radio_config = {
  .spi_speed   = 10000000,
  .ce_pin      = 22,
  .channel     = 76,
  .data_rate   = NRF24_DATA_RATE_1MBPS,
  .pa_level    = NRF24_PA_LEVEL_MIN,
  .crc_length  = NRF24_CRC_2_BYTES,
  .rtx_retries = 15,
  .rtx_delay   = 2,
};
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: HELPERS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void nrf24_get_spi_speed(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%.2f MHz", radio_config.spi_speed / 1e6);
    return;
}

void nrf24_get_channnel(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%d (%d MHz)", radio_config.channel, 2400 + radio_config.channel);
    return;
}

const char* nrf24_get_data_rate(void) {
    switch (radio_config.data_rate) {
        case NRF24_DATA_RATE_250KBPS: return  "250 KBps";
        case NRF24_DATA_RATE_1MBPS:   return    "1 MBps";
        case NRF24_DATA_RATE_2MBPS:   return    "2 MBps";
        default:                      return "UNDEFINED";
    }
}

const char* nrf24_get_pa_level(void) {
    switch (radio_config.pa_level) {
        case NRF24_PA_LEVEL_MIN:  return "-18 dBm (MIN)";
        case NRF24_PA_LEVEL_LOW:  return "-12 dBm (LOW)";
        case NRF24_PA_LEVEL_HIGH: return "-6 dBm (HIGH)";
        case NRF24_PA_LEVEL_MAX:  return   "0 dBm (MAX)";
        default:                  return     "UNDEFINED";
    }
}

const char* nrf24_get_crc_length(void) {
    switch (radio_config.crc_length) {
        case NRF24_CRC_NONE:    return       "OFF";
        case NRF24_CRC_1_BYTES: return    "1 BYTE";
        case NRF24_CRC_2_BYTES: return   "2 BYTES";
        default:                return "UNDEFINED";
    }
}

void nrf24_get_rtx_delay(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%d (%d us)", radio_config.rtx_delay, 250 * (1 + radio_config.rtx_delay));
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




// :::: RADIO CONFIG ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int nrf24_set_channel(int channel) {
    if (channel < 0 || 125 < channel) {

        return 1;
    }

    radio_config.channel = channel;
    // TODO: change channel

    return 0;
}

int nrf24_set_data_rate(nrf24_data_rate data_rate) {
    switch(data_rate) {
        case NRF24_DATA_RATE_250KBPS:
        case NRF24_DATA_RATE_1MBPS:
        case NRF24_DATA_RATE_2MBPS:
            radio_config.data_rate = data_rate;

            // TODO: change data rate

            return 0;

        case NRF24_DATA_RATE_UNSET:
        default:
            return 1;
    }
}

int nrf24_set_pa_level(nrf24_pa_level pa_level) {
    switch(pa_level) {
        case NRF24_PA_LEVEL_MIN:
        case NRF24_PA_LEVEL_LOW:
        case NRF24_PA_LEVEL_HIGH:
        case NRF24_PA_LEVEL_MAX:
            radio_config.pa_level = pa_level;

            // TODO: change pa level

            return 0;

        case NRF24_PA_LEVEL_UNSET:
        default:
            return 1;
    }
}

int nrf24_set_crc_length(nrf24_crc_length crc_length) {
    switch(crc_length) {
        case NRF24_CRC_NONE:
        case NRF24_CRC_1_BYTES:
        case NRF24_CRC_2_BYTES:
            radio_config.crc_length = crc_length;

            // TODO: change crc length

            return 0;

        case NRF24_CRC_UNSET:
        default:
            return 1;
    }
    return 0;
}

int nrf24_set_rtx_retries(int rtx_retries) {
    if (rtx_retries < 0 || 15 < rtx_retries) {
        return 1;
    }
    radio_config.rtx_retries = rtx_retries;
    // TODO: change rtx retries
    return 0;
}

int nrf24_set_rtx_delay(int rtx_delay) {
    if (rtx_delay < 0 || 15 < rtx_delay) {
        return 1;
    }
    radio_config.rtx_delay = rtx_delay;
    // TODO: change rtx delay
    return 0;
}

void nrf24_print_user_radio_config(void) {
    char spi_speed[128], channel[128], rtx_delay[128];
    nrf24_get_spi_speed(spi_speed, sizeof(spi_speed));
    nrf24_get_channnel(channel, sizeof(channel));
    nrf24_get_rtx_delay(rtx_delay, sizeof(rtx_delay));

    logger_log(LOGGER_INFO,
        "USER-DEFINED RADIO DETAILS:\n"
        "Hardware:\n"
        "    %-11s : %14s\n"  // SPI SPEED
        "    %-11s : %14d\n"  // CE PIN
        "\n"
        "Radio:\n"
        "    %-11s : %14s\n"  // CHANNEL
        "    %-11s : %14s\n"  // DATA RATE
        "    %-11s : %14s\n"  // PA LEVEL
        "    %-11s : %14s\n"  // CRC LENGTH
        "\n"
        "Reliability:\n"
        "    %-11s : %14d\n"  // RTX RETRIES
        "    %-11s : %14s\n", // RTX DELAY
        "SPI SPEED",   spi_speed,
        "CE PIN",      radio_config.ce_pin,
        "CHANNEL",     channel,
        "DATA RATE",   nrf24_get_data_rate(),
        "PA LEVEL",    nrf24_get_pa_level(),
        "CRC LENGTH",  nrf24_get_crc_length(),
        "RTX RETRIES", radio_config.rtx_retries,
        "RTX DELAY",   rtx_delay
    );
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

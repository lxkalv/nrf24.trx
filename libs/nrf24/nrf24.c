// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <stdio.h>

#include "logger/logger.h"

#include "nrf24.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#define DEFAULT_SPI_SPEED                    10.F // MHz
#define DEFAULT_CE_PIN                         22
#define DEFAULT_CHANNEL                        76
#define DEFAULT_DATA_RATE   NRF24_DATA_RATE_1MBPS
#define DEFAULT_PA_LEVEL       NRF24_PA_LEVEL_MIN
#define DEFAULT_CRC_LENGTH      NRF24_CRC_2_BYTES
#define DEFAULT_RTX_RETRIES                    15
#define DEFAULT_RTX_DELAY                       2

nrf24_config_t nrf24_config = {0};
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: HELPERS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
static void get_spi_speed_str(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%.2f MHz", nrf24_config.spi_speed / 1e6);
    return;
}

static void get_channnel_str(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%d (%d MHz)", nrf24_config.channel, 2400 + nrf24_config.channel);
    return;
}

static const char* get_data_rate_str(void) {
    switch (nrf24_config.data_rate) {
        case NRF24_DATA_RATE_250KBPS: return  "250 KBps";
        case NRF24_DATA_RATE_1MBPS:   return    "1 MBps";
        case NRF24_DATA_RATE_2MBPS:   return    "2 MBps";
        default:                      return "UNDEFINED";
    }
}

static const char* get_pa_level_str(void) {
    switch (nrf24_config.pa_level) {
        case NRF24_PA_LEVEL_MIN:  return "-18 dBm (MIN)";
        case NRF24_PA_LEVEL_LOW:  return "-12 dBm (LOW)";
        case NRF24_PA_LEVEL_HIGH: return "-6 dBm (HIGH)";
        case NRF24_PA_LEVEL_MAX:  return   "0 dBm (MAX)";
        default:                  return     "UNDEFINED";
    }
}

static const char* get_crc_length_str(void) {
    switch (nrf24_config.crc_length) {
        case NRF24_CRC_NONE:    return       "OFF";
        case NRF24_CRC_1_BYTES: return    "1 BYTE";
        case NRF24_CRC_2_BYTES: return   "2 BYTES";
        default:                return "UNDEFINED";
    }
}

static void get_rtx_retries_str(char* buffer, int capacity) {
    if (nrf24_config.rtx_retries)
        snprintf(buffer, capacity, "OFF");
    else
        snprintf(buffer, capacity, "%d", nrf24_config.rtx_retries);
    return;
}

static void get_rtx_delay_str(char* buffer, int capacity) {
    snprintf(buffer, capacity, "%d (%d us)", nrf24_config.rtx_delay, 250 * (1 + nrf24_config.rtx_delay));
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




// :::: RADIO CONFIG ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int nrf24_set_spi_speed(float spi_speed) {
    if (spi_speed < 1 || 10 < spi_speed) return 1;

    nrf24_config.spi_speed = (int)(spi_speed * 1e6);

    // TODO: change spi speed

    return 0;
}

int nrf24_set_ce_pin(int ce_pin) {
    if (ce_pin < 0 || 32 < ce_pin) return 1;

    nrf24_config.ce_pin = ce_pin;

    // TODO: change ce pin

    return 0;
}

int nrf24_set_channel(int channel) {
    if (channel < 0 || 125 < channel) return 1;

    nrf24_config.channel = channel;

    // TODO: change channel

    return 0;
}

int nrf24_set_data_rate(nrf24_data_rate_t data_rate) {
    switch(data_rate) {
        case NRF24_DATA_RATE_250KBPS:
        case NRF24_DATA_RATE_1MBPS:
        case NRF24_DATA_RATE_2MBPS:
            nrf24_config.data_rate = data_rate;

            // TODO: change data rate

            return 0;

        case NRF24_DATA_RATE_UNSET:
        default:
            return 1;
    }
}

int nrf24_set_pa_level(nrf24_pa_level_t pa_level) {
    switch(pa_level) {
        case NRF24_PA_LEVEL_MIN:
        case NRF24_PA_LEVEL_LOW:
        case NRF24_PA_LEVEL_HIGH:
        case NRF24_PA_LEVEL_MAX:
            nrf24_config.pa_level = pa_level;

            // TODO: change pa level

            return 0;

        case NRF24_PA_LEVEL_UNSET:
        default:
            return 1;
    }
}

int nrf24_set_crc_length(nrf24_crc_length_t crc_length) {
    switch(crc_length) {
        case NRF24_CRC_NONE:
        case NRF24_CRC_1_BYTES:
        case NRF24_CRC_2_BYTES:
            nrf24_config.crc_length = crc_length;

            // TODO: change crc length

            return 0;

        case NRF24_CRC_UNSET:
        default:
            return 1;
    }
    return 0;
}

int nrf24_set_rtx_retries(int rtx_retries) {
    if (rtx_retries < 0 || 15 < rtx_retries) return 1;

    nrf24_config.rtx_retries = rtx_retries;

    // TODO: change rtx retries

    return 0;
}

int nrf24_set_rtx_delay(int rtx_delay) {
    if (rtx_delay < 0 || 15 < rtx_delay) return 1;

    nrf24_config.rtx_delay = rtx_delay;

    // TODO: change rtx delay

    return 0;
}

void nrf24_apply_default_config(void) {
    nrf24_set_spi_speed(DEFAULT_SPI_SPEED);
    nrf24_set_ce_pin(DEFAULT_CE_PIN);
    nrf24_set_channel(DEFAULT_CHANNEL);
    nrf24_set_data_rate(DEFAULT_DATA_RATE);
    nrf24_set_pa_level(DEFAULT_PA_LEVEL);
    nrf24_set_crc_length(DEFAULT_CRC_LENGTH);
    nrf24_set_rtx_retries(DEFAULT_RTX_RETRIES);
    nrf24_set_rtx_delay(DEFAULT_RTX_DELAY);
    return;
}

void nrf24_print_user_radio_config(void) {
    char spi_speed[128], channel[128], rtx_retries[128], rtx_delay[128];
    get_spi_speed_str(spi_speed, sizeof(spi_speed));
    get_channnel_str(channel, sizeof(channel));
    get_rtx_retries_str(rtx_retries, sizeof(rtx_retries));
    get_rtx_delay_str(rtx_delay, sizeof(rtx_delay));

    logger_log(LOGGER_INFO,
        "USER-DEFINED RADIO CONFIG:\n"
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
        "    %-11s : %14s\n"  // RTX RETRIES
        "    %-11s : %14s\n", // RTX DELAY
        "SPI SPEED",   spi_speed,
        "CE PIN",      nrf24_config.ce_pin,
        "CHANNEL",     channel,
        "DATA RATE",   get_data_rate_str(),
        "PA LEVEL",    get_pa_level_str(),
        "CRC LENGTH",  get_crc_length_str(),
        "RTX RETRIES", rtx_retries,
        "RTX DELAY",   rtx_delay
    );
    return;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

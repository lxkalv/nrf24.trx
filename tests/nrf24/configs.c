#include <linux/limits.h>
#include <stdio.h>

#include "logger/logger.h"
#include "nrf24/nrf24.h"

int main() {
    logger_log(LOGGER_INFO, "NRF24 0 initialized config:\n");
    nrf24_print_user_radio_config();

    printf("\n");

    logger_log(LOGGER_INFO, "NRF24 default config:\n");
    nrf24_apply_default_config();
    nrf24_print_user_radio_config();

    printf("\n");

    logger_log(LOGGER_INFO, "NRF24 custom config:\n");
    nrf24_set_spi_speed(5.5);
    nrf24_set_ce_pin(12);
    nrf24_set_channel(21);
    nrf24_set_data_rate(NRF24_DATA_RATE_2MBPS);
    nrf24_set_pa_level(NRF24_PA_LEVEL_HIGH);
    nrf24_set_crc_length(NRF24_CRC_1_BYTES);
    nrf24_set_rtx_retries(2);
    nrf24_set_rtx_delay(15);
    nrf24_print_user_radio_config();
    return 0;
}

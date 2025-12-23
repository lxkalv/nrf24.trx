#ifndef   NRF24_H
#define   NRF24_H

// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Configuration
typedef enum {
    NRF24_DATA_RATE_250KBPS = 0,
    NRF24_DATA_RATE_1MBPS,
    NRF24_DATA_RATE_2MBPS,
} nrf24_data_rate;

typedef enum {
    NRF24_PA_LEVEL_MIN = 0,
    NRF24_PA_LEVEL_LOW,
    NRF24_PA_LEVEL_HIGH,
    NRF24_PA_LEVEL_MAX,
} nrf24_pa_level;

typedef enum {
    NRF24_CRC_NONE = 0,
    NRF24_CRC_1_BYTES,
    NRF24_CRC_2_BYTES,
} nrf24_crc_length;

typedef struct {
    int              spi_speed;
    int              ce_pin;
    int              channel;
    nrf24_data_rate  data_rate;
    nrf24_pa_level   pa_level;
    nrf24_crc_length crc_length;
    int              retransmission_retries;
    int              retransmission_delay;
} nrf24_config;

extern nrf24_config radio_config;
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: RADIO CONFIG ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void nrf24_print_user_radio_config(void);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // NRF24_H

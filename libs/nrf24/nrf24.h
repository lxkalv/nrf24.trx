#ifndef   NRF24_H
#define   NRF24_H

// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Enum representing the possible data rate of the NRF24L01+ module.
 */
typedef enum {
    NRF24_DATA_RATE_UNSET = -1,
    NRF24_DATA_RATE_250KBPS,
    NRF24_DATA_RATE_1MBPS,
    NRF24_DATA_RATE_2MBPS,
} nrf24_data_rate_t;

/**
 * @brief Enum representing the possible pa levels of the NRF24L01+ module.
 */
typedef enum {
    NRF24_PA_LEVEL_UNSET = -1,
    NRF24_PA_LEVEL_MIN,
    NRF24_PA_LEVEL_LOW,
    NRF24_PA_LEVEL_HIGH,
    NRF24_PA_LEVEL_MAX,
} nrf24_pa_level_t;

/**
 * @brief Enum representing the possible crc lengths of the NRF24L01+ module.
 */
typedef enum {
    NRF24_CRC_UNSET = -1,
    NRF24_CRC_NONE,
    NRF24_CRC_1_BYTES,
    NRF24_CRC_2_BYTES,
} nrf24_crc_length_t;

/**
 * @brief Struct holding the NRF24L01+ module configuration.
 *
 * @param spi_speed The speed at which the MCU interacts with the module
 * @param ce_pin The pin used to communicate with the module
 * @param channel The channel to be used in the communication (only for p2p protocol)
 * ... TODO
 */
typedef struct {
    int                spi_speed;
    int                ce_pin;
    int                channel;
    nrf24_data_rate_t  data_rate;
    nrf24_pa_level_t   pa_level;
    nrf24_crc_length_t crc_length;
    int                rtx_retries;
    int                rtx_delay;
} nrf24_config_t;

extern nrf24_config_t nrf24_config;
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: RADIO CONFIG ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int nrf24_set_spi_speed(float spi_speed);

int nrf24_set_ce_pin(int ce_pin);

int nrf24_set_channel(int channel);

int nrf24_set_data_rate(nrf24_data_rate_t data_rate);

int nrf24_set_pa_level(nrf24_pa_level_t pa_level);

int nrf24_set_crc_length(nrf24_crc_length_t crc_length);

int nrf24_set_rtx_retries(int rtx_retries);

int nrf24_set_rtx_delay(int rtx_delay);

void nrf24_apply_default_config(void);

void nrf24_print_user_radio_config(void);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // NRF24_H

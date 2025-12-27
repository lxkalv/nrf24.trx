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

/**
 * @brief Set the spi communication speed with the device.
 *
 * @param spi_speed The speed to be set, in MHz {1..10}
 */
int nrf24_set_spi_speed(float spi_speed);

/**
 * @brief Set CE pin of the communication with the device.
 *
 * @param ce_pin The pin index to be used {idk yet}
 */
int nrf24_set_ce_pin(int ce_pin);

/**
 * @brief Set the RF channel to be used in the communication.
 *
 * @param channel The channel to be used {0..125}
 */
int nrf24_set_channel(int channel);

/**
 * @brief Set the on-air data rate of the communication.
 *
 * @param data_rate The data rate to be used
 */
int nrf24_set_data_rate(nrf24_data_rate_t data_rate);

/**
 * @brief Set the PA Power level of the communication.
 *
 * @param pa_level The power to be used
 */
int nrf24_set_pa_level(nrf24_pa_level_t pa_level);

/**
 * @brief Set the CRC length for frame checking
 *
 * @param crc_length The length to be used
 */
int nrf24_set_crc_length(nrf24_crc_length_t crc_length);

/**
 * @brief Set the maximum number of retries to send a frame.
 *
 * @param rtx_retries The number of retries to be used {0..15}
 */
int nrf24_set_rtx_retries(int rtx_retries);

/**
 * @brief Set the delay between frame retransmissions.
 *
 * @param rtx_delay The delay to be used {0..15}
 */
int nrf24_set_rtx_delay(int rtx_delay);

/**
 * @brief Apply the default config to the module
 *
 * The default config is defined as:
 *     - SPI SPEED: 10MHz
 *     - CE PIN: 22
 *     - CHANNEL: 76
 *     - DATA RATE: 1MBPS
 *     - PA LEVEL: MIN
 *     - CRC LENGTH: 2 BTYES
 *     - RTX RETRIES: 15
 *     - RTX DELAY: 2
 */
void nrf24_apply_default_config(void);

/**
 * @brief Print the current radio config to the screen
 */
void nrf24_print_user_radio_config(void);

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // NRF24_H

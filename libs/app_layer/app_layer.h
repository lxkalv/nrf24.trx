#ifndef   APP_LAYER_H
#define   APP_LAYER_H

// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Enum representing the role that the receiver will take.
 */
typedef enum {
    APP_MODE_UNSET = -1,
    APP_MODE_TX,
    APP_MODE_RX,
} app_mode;

/**
 * @brief Enum representing the app protocol configuration.
 */
typedef enum {
    APP_PROTOCOL_UNSET = -1,
    APP_PROTOCOL_P2P,
    APP_PROTOCOL_NETWORK,
} app_protocol;

/**
 * @brief Struct holding the app configuration based on user input.
 *
 * @param mode Representation of the role of the current node
 * @param protocol Representation of the current protocol used for the communication
 * @param file_path Optional file path pointing to the file to send/store
 */
typedef struct {
    app_mode     mode;
    app_protocol protocol;
    char         file_path[512];
} app_config_t;

extern app_config_t app_config;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: ARG PARSING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
 * @brief Parse the user/radio configuration from the command line.
 *
 * This function parses the command line arguments and configures the app and the
 * radio module based on the user input. It makes use of the nrf24 API to validate
 * the arguments.
 *
 * @param argc The number of arguments provided in the command line
 * @param argv The values of the arguments provided in the command line
 *
 * @return 0 on success, 1 on error or help flag
 */
int app_parse_user_config(int argc, char** argv);


/**
 * @brief Print the current app configuration.
 */
void app_print_app_config(void);

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: LED CONTROL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TODO: ...
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // APP_LAYER_H

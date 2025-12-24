#ifndef   APP_LAYER_H
#define   APP_LAYER_H

// :::: LIBRARY IMPORTS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include "nrf24/nrf24.h"
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: DEFINITIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef enum {
    APP_MODE_UNSET = -1,
    APP_MODE_TX,
    APP_MODE_RX,
} app_mode;

typedef enum {
    APP_PROTOCOL_UNSET = -1,
    APP_PROTOCOL_P2P,
    APP_PROTOCOL_NETWORK,
} app_protocol;

typedef struct {
    app_mode     mode;
    app_protocol protocol;
} app_config;

extern app_config user_config;
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





// :::: ARG PARSING :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int app_read_user_config_from_args(int argc, char** argv);

void app_print_user_config(void);
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // APP_LAYER_H

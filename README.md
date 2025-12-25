# nrf24.tx
A revisited version of `nrf24.mtp`. A project developed for the UPC METT MTP
subject.

## Layers
In order to facilitate the development of the available file transmission
protocols, a series of layers were developed trying to follow the OSI standard.
Here we present each of them, explaining their responsabilities and interfaces.

### Application Layer (`app_layer.[h|c]`)
This layer is responsible of interfacing with the user by means of the command
line, controlling the LEDs of the node and moving the neccessary files from and
to a USB.

It holds the app configuration provided by the user in the `app_config` struct.
It also passes the radio configuration to the nrf24 module.

```
┏━━━━━━━━━━━━━━━━━━━┓
┃ APPLICATION LAYER ┃
┗━━━━━━━━━━━━━━━━━━━┛
```

### Presentation Layer (`pres_layer.[h|c]`)
This layer is responsible for the read/write operations of the file, splitting
and merging the contents of the file and compressing and decompressing the data.

It stores it state in the `pres_layer_state`struct.

```
┏━━━━━━━━━━━━━━━━━━━━┓
┃ PRESENTATION LAYER ┃
┗━━━━━━━━━━━━━━━━━━━━┛
```

## Protocols

### P2P Architecture
### Network

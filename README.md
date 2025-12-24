# nrf24.tx
A revisited version of `nrf24.mtp`. A project developed for the UPC METT MTP
subject.

# Layers
In order to facilitate the development of the available file transmission
protocols, a series of layers were developed trying to follow the OSI standard.
Here we present each of them, explaining their responsabilities and interfaces.

## Application Layer (`app_layer.[h|c]`)
This layer is responsible of interfacing with the user by means of the command
line, controlling the LEDs of the node and moving the neccessary files from and
to a USB.

```
┏━━━━━━━━━━━━━━━━━━━┓
┃ APPLICATION LAYER ┃
┗━━━━━━━━━━━━━━━━━━━┛
```

## P2P Architecture
## Network

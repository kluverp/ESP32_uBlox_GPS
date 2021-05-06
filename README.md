# GPS #

Gps component for easy readout of GPS coordinates in latitude/longitude format.

This component is compatible with the uBlox NEO-6 series.

### Requirements

* IDF v4.2
* C++

### How do I get set up? ###

Initialize the Gps object, and pass in your UART_NUM to use and Tx, Rx lines to use.
The Tx line is never used, so you can pass in any Tx line.

The Rx line is the only one used, since we only read from the Gps module.

```
// create new object
class Gps Gps;

// init the Gps object
Gps.init(GPS_UART_NUM, GPS_UART_TX, GPS_UART_RX);
```

After **init()** is called, this spawns a new freeRTOS task handling the UART data by using a pattern detection.

### Lat / long

The module reports different NMEA frames, but only the **GPGLL** address is used to read the lat/lng data.
You can write you own parser for other addresses if you want.

After init, you can call:
```
Gps.getLat();
Gps.getLng();
```
To get lat/lng in **float** format. 

If you like string format, you can do:

```
Gps.getLatStr();
Gps.getLngStr();
```

### Other NMEA addresses

The code can be extended to support more NMEA addresses. However, I only needed the lat/long data
so the reset is not implemented.

It is not 'complete' so to say...

#ifndef AOS_GPS_H
#define AOS_GPS_H

#include <string>
#include "esp_log.h"
#include "driver/uart.h"
#include "aos_nmea_frame.h"
#include "aos_nmea_gpgll.h"

class Gps {
private:
    uart_port_t port_ = UART_NUM_1;
    QueueHandle_t uart_queue_;
    float lat_ = 0;
    float lng_ = 0;

public:
    static const char *TAG;

    /**
     * Init GPS object
     *
     * @param port The UART port number to use
     * @param tx Tx GPIO pin
     * @param rx Rx GPIO pin
     */
    void init(uart_port_t port, int tx, int rx);

    /**
     * Handles the data from UART event handler.
     *
     * @param std::string The data string from uBlox module.
     */
    void handleData(std::string);

    /**
     * Returns the Latitude when set.
     *
     * @return float
     */
    float getLat(void);

    /**
     * Returns the Longitude when set.
     *
     * @return float
     */
    float getLng(void);

    /**
     * Returns the Latitude in string format.
     *
     * @return std::string
     */
    std::string getLatStr();

    /**
     * Returns the Longitude in string format.
     *
     * @return std::string
     */
    std::string getLngStr();

    /**
     * Returns the UART port number.
     *
     * @return uart_port_t
     */
    uart_port_t getUartNum();
};

#endif // AOS_GPS_H

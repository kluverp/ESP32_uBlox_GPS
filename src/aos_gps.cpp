#include <cstring>
#include "aos_gps.h"

QueueHandle_t uart0_queue;

#define RD_BUF_SIZE (1024)
#define PATTERN_CHR_NUM 1

const char *Gps::TAG = "GPS";

static void uart_event_task(void *pvParameters) {
    // get our object passed in via a pointer
    Gps *gps = (Gps *) pvParameters;

    uart_event_t event;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *) malloc(RD_BUF_SIZE);
    int pos = 0;
    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void *) &event, (portTickType) portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            // ESP_LOGI("TEST", "uart[%d] event:", gps->getUartNum());
            switch (event.type) {
                //Event of UART receiving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    //ESP_LOGI(Nextion::TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(gps->getUartNum(), dtmp, event.size, portMAX_DELAY);
                    break;
                    //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    //ESP_LOGI(Nextion::TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(gps->getUartNum());
                    xQueueReset(uart0_queue);
                    break;
                    //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    //ESP_LOGI(Nextion::TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(gps->getUartNum());
                    xQueueReset(uart0_queue);
                    break;
                    //Event of UART RX break detected
                case UART_BREAK:
                    //ESP_LOGI(Nextion::TAG, "uart rx break");
                    break;
                    //Event of UART parity check error
                case UART_PARITY_ERR:
                    // ESP_LOGI(Nextion::TAG, "uart parity error");
                    break;
                    //Event of UART frame error
                case UART_FRAME_ERR:
                    //ESP_LOGI(Nextion::TAG, "uart frame error");
                    break;
                    //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    uart_get_buffered_data_len(gps->getUartNum(), &buffered_size);
                    pos = uart_pattern_pop_pos(gps->getUartNum());
                    // ESP_LOGI("TEST", "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                    if (pos == -1) {
                        // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                        // record the position. We should set a larger queue size.
                        // As an example, we directly flush the rx buffer here.
                        uart_flush_input(gps->getUartNum());
                    } else {
                        uart_read_bytes(gps->getUartNum(), dtmp, pos, 100 / portTICK_PERIOD_MS);
                        uint8_t pat[PATTERN_CHR_NUM + 1];
                        memset(pat, 0, sizeof(pat));
                        uart_read_bytes(gps->getUartNum(), pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                        gps->handleData(std::string((const char *) dtmp));
//                        ESP_LOGI("TEST", "read data: %s", dtmp);
//                        ESP_LOGI("TEST", "read pat : %s", pat);
                    }
                    break;
                    //Others
                default:
                    ESP_LOGI("TEST", "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void Gps::init(uart_port_t port, int tx, int rx) {
    port_ = port;
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
            .rx_flow_ctrl_thresh = 122,
    };

    ESP_ERROR_CHECK(uart_param_config(port_, &uart_config));

    // Set UART pins(TX: IO17 (UART2 default), RX: IO16 (UART2 default), RTS: IO18, CTS: IO19)
    ESP_ERROR_CHECK(uart_set_pin(port_, tx, rx, 0, 0));

    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);

    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(port_, uart_buffer_size, uart_buffer_size, 10, &uart0_queue, 0));

    // Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(port_, '\n', 1, 9, 0, 0);

    // Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(port_, 20);

    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 3072, this, 12, NULL);
}

void Gps::handleData(std::string data) {
    NmeaFrame frame(data.c_str(), data.length());
    if (frame.isValid()) {
        if (strcmp(frame.getAddress().c_str(), "GPGLL") == 0) {
            NmeaGpgll gpgll(frame.getData());
            if (gpgll.isValid()) {
                lat_ = gpgll.getLatDD();
                lng_ = gpgll.getLngDD();
            }
        }
    }
}

float Gps::getLat() {
    return lat_;
}

float Gps::getLng() {
    return lng_;
}

std::string Gps::getLatStr() {
    char str[20];
    snprintf(str, sizeof(str), "%f", getLat());
    return std::string(str);
}

std::string Gps::getLngStr() {
    char str[20];
    snprintf(str, sizeof(str), "%f", getLng());
    return std::string(str);
}

uart_port_t Gps::getUartNum() {
    return port_;
}
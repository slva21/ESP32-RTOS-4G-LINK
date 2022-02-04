#include <Arduino.h>
#include "./SPI_LINK/link.hh"
#include "./TCP_LINK/link.hh"
#include "../src\MAV_PARSER\mav_data_handler.hh"
#include "../src/MAV_PARSER/mav_cmd_parser.hh"
#include "../src/MAV_PARSER/mav_msg_encoder.hh"

// Use only one core for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

WiFiClient client;

// Task handlers
static TaskHandle_t task_1_mav_tcp = NULL;
static TaskHandle_t task_2_mav_serial = NULL;

// Timer handle
static TimerHandle_t timer_1_mav_heatbeat = nullptr;

SPI_LINK COM;
TCP_LINK TCP;

#define DEBUG 1

//MAV_CMD


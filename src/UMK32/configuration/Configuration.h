#pragma once

#define DEVICE_MAC {0x00, 0x01, 0x02, 0x02, 0x01, 0x12}
#define DEVICE_NAME F("UMK32_System")
#define DEVICE_PASS F("admin")


// NET
#define NET_DEFAULT_IP_1 192
#define NET_DEFAULT_IP_2 168
#define NET_DEFAULT_IP_3 0
#define NET_DEFAULT_IP_4 31

#define NET_DEFAULT_MASK_1 255
#define NET_DEFAULT_MASK_2 255
#define NET_DEFAULT_MASK_3 255
#define NET_DEFAULT_MASK_4 0

#define NET_DEFAULT_GATEWAY_1 192
#define NET_DEFAULT_GATEWAY_2 168
#define NET_DEFAULT_GATEWAY_3 0
#define NET_DEFAULT_GATEWAY_4 1

// LIMITS
#define SENSORS_MAX 16
#define DELTAS_MAX 16
#define NAMES_MAX_LEN 10
#define DEVICE_PARAM_MAX_LEN 100


// OTHER
#define SENS_ERR_VALUE -255.0 // не входит в рабочий диапазон температур 
#define SENS_STATUS_OK 2
#define SENS_STATUS_OFF 1
#define SENS_STATUS_ERROR 0

#define PRINT_PART_SIZE 5 

#define WDT_TIMEOUT 30




// TIMERS
// NOTE: НЕ ПОНИМАЕШЬ ПОЧЕМУ ТАКИЕ ЧИСЛА - РУКИ ПРОЧЬ!!!!
#define SENS_READ_DELAY 750 // задержка на считывание датчика (мс)
#define SENS_READ_PERIOD 30 // частота обновления датчиков (сек)

// EEPROM
#define EEPROM_IP_ADDR 1
#define EEPROM_MASK_ADDR 5
#define EEPROM_GATE_ADDR 9

#define EEPROM_ERRTIMEOUT_ADDR 13
#define EEPROM_TIMESET_ADDR 15

#define EEPROM_PASSWORD_ADDR 100
#define EEPROM_DEVICENAME_ADDR 200

#define EEPROM_DELTAS_START_ADDR 2000
#define EEPROM_SENSORS_START_ADDR 1000
#define EEPROM_SIZE 3000


// PINOUT
#define SENS_PINS_NUM 2

#define ETH_PIN_CS 5

#ifndef ETH_PHY_TYPE
#define ETH_PHY_TYPE ETH_PHY_W5500
#define ETH_PHY_ADDR 1
#define ETH_PHY_CS   5
#endif

// SPI pins
#define ETH_SPI_SCK  18
#define ETH_SPI_MISO 19
#define ETH_SPI_MOSI 23



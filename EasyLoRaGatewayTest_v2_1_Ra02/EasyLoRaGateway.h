// ===================================================
// Put all pin mappings and library here
// ===================================================
// Serial
#define BAUD_RATE 9600  //9600

// Ethernet
#define ETH_PHY_MDIO 0
#define ETH_PHY_ADDR 1
#define ETH_PHY_MDC 16
#define ETH_PHY_POWER 33 // LAN_RST, too
#define ETH_PHY_TYPE ETH_PHY_LAN8720
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
//#define LAN_RST   33

// Must be after #define
#include <ETH.h>

// =====================
// LoRa
// =====================
#include <SPI.h>
#include <LoRa.h>

// LoRa 1 pins
#define LORA_SS          23
#define LORA_SCK         18
#define LORA_MOSI         5
#define LORA_MISO        36
#define LORA_DIO0        39
#define LORA_RST         13

// LoRa 2 pins
#define LORA2_SS         32
#define LORA2_SCK        18 // Shared
#define LORA2_MOSI        5 // Shared
#define LORA2_MISO       36 // Shared
#define LORA2_DIO0       34
#define LORA2_RST        15

// LoRa signals
#define LORA_SF 12 // 7 is the fastest. 12 is is farthest
#define LORA_CR 5
#define LORA_BW 41.7E3 //31.25E3 // 41.7E3 //125E3
#define LORA_PREAMBLE_LENGTH  8

// LED
#define LED 14

// Button
#define BTN 35

// Speaker
#define SPK 12
#define SPK_CHANNEL 0
#define SPK_FREQ  800
#define SPK_RESOLUTION  8

// GPIOs
#define IO2 2
#define IO4 4

// ===================================================
// Put all pin mappings and library here
// ===================================================
// Serial
#define BAUD_RATE             115200  //9600

// WiFi
#include <WiFi.h>
#define WIFI_NAME "Easy LoRa"   //  TO CHANGE TO YOUR WIFI
#define WIFI_PWD "EasyLoRa123"  //  TO CHANGE TO YOUR WIFI 
String WiFi_IP="0.0.0.0";

// Ethernet
#define ETH_PHY_MDIO          0
#define ETH_PHY_ADDR          1
#define ETH_PHY_MDC           16
#define ETH_PHY_POWER         33 // LAN_RST, too
#define ETH_PHY_TYPE          ETH_PHY_LAN8720
#define ETH_CLK_MODE          ETH_CLOCK_GPIO17_OUT
//#define LAN_RST             33

// Must be after #define
#include <ETH.h>
static bool eth_connected = false;
String ETH_Status;
String ETH_Ip;
String ETH_hostname;

// =====================
// LoRa
// =====================
#include <SPI.h>
#include <LoRa.h>

// LoRa 1 pins
#define LORA_SS                23
#define LORA_SCK               18 // Shared
#define LORA_MOSI              5 // Shared
#define LORA_MISO              36
#define LORA_DIO0              39
#define LORA_DIO1              4 // Not tested
#define LORA_RST               13

// LoRa signals
#define LORA_FREQ              920E6
#define LORA_SF                9 // 7 is the fastest. 12 is is farthest
#define LORA_CR                5
#define LORA_BW                250E3 // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3 and 500E3
#define LORA_PREAMBLE_LENGTH   8
#define LORA_LNA_GAIN          6 // 0-6: 0 to disable. 6 is max gain

// LoRa parameters
volatile bool isLoRaPacketReceived = false;

// LoRa 2 pins
#define LORA2_SS               32
#define LORA2_SCK              18 // Shared
#define LORA2_MOSI             5 // Shared
#define LORA2_MISO             36 // Shared
#define LORA2_DIO0             34
#define LORA2_DIO1             2 // Not tested
#define LORA2_RST              15

// LoRa 2 signals
#define LORA2_FREQ             920E6
#define LORA2_SF               9 // 7 is the fastest. 12 is is farthest
#define LORA2_CR               5
#define LORA2_BW               250E3 // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3 and 500E3
#define LORA2_PREAMBLE_LENGTH  8
#define LORA2_LNA_GAIN         6 // 0-6: 0 to disable. 6 is max gain

// 20dBm=100mW, 18dBm=63mW, 17dBm=50mW, 14dBm=25mW
#define LORA2_TXPOWER          17 // From 2 to 20. Default is 17.

// LED
#define LED                    14 // HIGH is ON

// Button
#define BTN                    35 // PRESSED is LOW

// Speaker
#define SPK                    12
#define SPK_CHANNEL            0
#define SPK_FREQ               800
#define SPK_RESOLUTION         8

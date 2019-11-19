// ===================================================
// Put all pin mappings and library here
// ===================================================
// =====================
// Serial
// =====================
#define BAUD_RATE 115200 //9600

// =====================
// Preferences
// =====================
#include <Preferences.h>
/* create an instance of Preferences library */
Preferences preferences;
unsigned int reset_times;

// =====================
// Ethernet
// =====================
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
// WiFi
// =====================
#include <WiFi.h>
const char* ssid     = "Easy LoRa";
const char* password = "EasyLoRa123";

// =====================
// Bluetooth
// =====================
#include "BluetoothSerial.h"
#include "esp_bt.h"
// Bluetooth Serial object
BluetoothSerial SerialBT;
bool isBTConnected = false;

// =====================
// LoRa 1 and 2
// =====================
#include <SPI.h>
#include <LoRa.h>

// LoRa 1 pins
#define LORA_SS          23
#define LORA_SCK         18
#define LORA_MOSI         5
#define LORA_MISO        36
#define LORA_DIO012      39
#define LORA_RST         13

// LoRa 2 pins
#define LORA2_SS         32
#define LORA2_SCK        18 // Shared
#define LORA2_MOSI        5 // Shared
#define LORA2_MISO       36 // Shared
#define LORA2_DIO012     34
#define LORA2_RST        15

// LoRa 1 signal configuration
#define LORA_FREQ 433E6
#define LORA_SF 7 // 7 is the fastest. 12 is is farthest
#define LORA_CR 5
#define LORA_BW 125E3 //31.25E3 // 41.7E3 //125E3
#define LORA_PREAMBLE_LENGTH  8

// LoRa 2 signal configuration
LoRaClass LoRa2;
#define LORA2_FREQ_BASE 433E6
#define LORA2_FREQ_STEPS 200E3 // Steps must be larger than BW
#define LORA2_FREQ_NUM_CHANNELS 19 // Max channels (not including base channel)
#define LORA2_SF 7 // 7 is the fastest. 12 is is farthest
#define LORA2_CR 5
#define LORA2_BW 125E3 //31.25E3 // 41.7E3 //125E3
#define LORA2_PREAMBLE_LENGTH  8

// =====================
// OTA
// =====================
#include <ArduinoOTA.h>

// =====================
// LED
// =====================
#define LED 14

// =====================
// Button
// =====================
#define BTN 35
volatile bool isBtnPressed = false;

// =====================
// Speaker
// =====================
#define SPK 12
#define SPK_CHANNEL 0
#define SPK_FREQ  800
#define SPK_RESOLUTION  8

// =====================
// GPIOs
// =====================
#define IO2 2
#define IO4 4

// =====================
// MQTT
// =====================
#include <MQTT.h>
#define MQTT_KEEP_ALIVE 5 // int, in seconds
#define MQTT_CLEAN_SESSION false // bool, "false" to resume existing session
#define MQTT_TIMEOUT 15000 // int, in miliseconds

#define MQTT_SERVER "easylora.vn"
#define MQTT_PORT 1883

#define MQTT_USERNAME "8nVPtDK5o5qdDJuSCG1Z" // Authentication code
#define MQTT_SECRET "8nVPtDK5o5qdDJuSCG1Z" // Any non-empty code is valid for ThingsBoard MQTT

// Publish to inform device is connected to the Gateway
#define MQTT_API_CONNECT "v1/gateway/connect"
#define MQTT_API_DISCONNECT "v1/gateway/disconnect"

// Publish and subscribe to attribute updates from the server
#define MQTT_API_ATTRIBUTE "v1/gateway/attributes"

// Publish to get client-side or shared attribute specific values from the server
// Subscribe before publish
#define MQTT_API_ATTRIBUTE_RESPONSE "v1/gateway/attributes/response"
#define MQTT_API_ATTRIBUTE_REQUEST "v1/gateway/attributes/request"

// Publish device telemetry
#define MQTT_API_TELEMETRY "v1/gateway/telemetry"

// Subscribe to RPC commands 
#define MQTT_API_RPC "v1/gateway/rpc"
// =====================
// ThingsBoard
// =====================
#define TB_HOST "easylora.vn"
#define TB_PORT 80
#define TB_HTTP_TIMEOUT 10000 // In ms

// =====================
// Utils - System
// =====================
#include <ArduinoJson.h>
unsigned int SYS_GatewayReqID = 0;
char SYS_ChipID[15];
char SYS_HostName[20];

// =====================
// Cron jobs
// =====================
#define CRONJOB_PRIORITY_READLORA 1
#define CRONJOB_PRIORITY_POLLING_NODE 0 // 0 is Idle, default is 1
unsigned long CRONJOB_START_MILLIS = 0;
unsigned long CRONJOB_CURRENT_MILLIS;
const unsigned long CRONJOB_POLLING_NODE_PERIOD = 15000; // miliseconds

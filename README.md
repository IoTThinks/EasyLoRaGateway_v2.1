# EasyLoRaGateway v2.1 (433Mhz and 915Mhz)
Easy LoRa gateway connects multiple smart IoT Plugs and Nodes in large buildings, factories and farms up to 10 km from the nearest Internet access. Easy LoRa gateway uses LoRa as the transmission protocol via 433+ Mhz/915+ Mhz wave. It enables us to remotely control our smart IoT Plugs and Nodes inexpensively and efficiently.
- A 10-storey building only needs 01 Easy LoRa gateway to connects to all LoRa Plugs and sensors in the building. A 10-hectare farm may only needs 01 LoRa Gateway to reach all of its IoT devices.
- Homepage: http://iotthinks.com/easy-lora-gateway/
- Full Wiki: https://github.com/IoTThinks/EasyLoRaGateway_v2.1/wiki

Previous version:
- Old version of Easy LoRa gateway v1 is at https://github.com/IoTThinks/EasyLoraGateway
- Old version of Easy LoRa gateway v2 is at https://github.com/IoTThinks/EasyLoRaGateway_v2

## 1. Hardware specification
- 1 x ESP32 chip with WiFi 802.11n and BLE
- 2 x LoRa SX1278 433MHz (Ai-Thinker Ra02) or 915Mhz (RFM95W)
- 1 x 100 Mbps Ethernet LAN
- 1 x Ethernet Console (for debug and upload firmware)
- 1 x Speaker
- 1 x LED
- 1 x Power LED
- 1 x 5v Power jack
- Over voltage protection, reverse-polarity protection, short-circuit protection via PTC fuse

## 2. Appearance 
From top
![Easy LoRa Gateway - Top](https://user-images.githubusercontent.com/29994971/63238055-d5c29100-c26e-11e9-8cbc-09ef7bee3845.JPG)

From bottom
![Easy LoRa v2 1 - bottom](https://user-images.githubusercontent.com/29994971/63233301-ace3d100-c259-11e9-9a7b-c0d4757752c0.JPG)

From inside - Ai-Thinker Ra02 (433Mhz)
![Easy LoRa v2 1 - inside](https://user-images.githubusercontent.com/29994971/63233300-ace3d100-c259-11e9-8989-f151fbd00a25.JPG)

From inside - RFM95W (915Mhz)
![Easy LoRa Gateway v2 1 - RFM95W - Components](https://user-images.githubusercontent.com/29994971/104901891-90007f80-59b0-11eb-8f50-9a383154f43f.jpg)

## 3. Pin mappings
### 3.1 Ai-Thinker Ra02 version
Pin mappings from main ESP32 to components
![Easy LoRa Gateway v2 1 - Pin mapping](https://user-images.githubusercontent.com/29994971/63233171-c59fb700-c258-11e9-8842-81c59109e307.png)

### 3.2 RFM95W version
Pin mappings from main ESP32 to components
![Easy LoRa Gateway v2 1 RFM95W - Pin mapping](https://user-images.githubusercontent.com/29994971/104907733-22f0e800-59b8-11eb-9b6b-f0ee3d424dfb.png)

## 4. Pinout of MCU and LoRa
### 4.1 Pinout of ESP32
Datasheet: [esp32-wroom-32_datasheet_en.pdf](https://github.com/IoTThinks/EasyLoRaGateway_v2.1/files/5829820/esp32-wroom-32_datasheet_en.pdf)

![image](https://user-images.githubusercontent.com/29994971/60634413-07a3a380-9e39-11e9-86dd-2acc5d586cae.png)

### 4.2 Pinout of LoRa Ai-Thinker RA-02
Datasheet: [LoRa-SX1278-Datasheet.pdf](https://github.com/IoTThinks/EasyLoRaGateway_v2.1/files/5829814/LoRa-SX1278-Datasheet.pdf)
![ra-02](https://user-images.githubusercontent.com/29994971/63237606-e07c2680-c26c-11e9-81fe-7d4bf97f237a.png)

### 4.3 Pinout of LoRa RFM95W
Datasheet: [RFM95W-V2.0.pdf](https://github.com/IoTThinks/EasyLoRaGateway_v2.1/files/5829531/RFM95W-V2.0.pdf)

![rfm95w](https://user-images.githubusercontent.com/29994971/104902531-6eec5e80-59b1-11eb-900c-c7f35da2d57c.png)


## 5. Software feature (In progress)
- Single or dual channel LoRa gateway
- Simple local management portal
- Auto provision of Easy LoRa Plug and Node
- Forward messages from LoRa to MQTT servers
- Web Service APIs for integration
- Auto update firmware via LAN or Internet
- OPEN SOURCE and commercial version

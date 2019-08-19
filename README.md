# EasyLoRaGateway_v2.1
Easy LoRa gateway connects multiple smart IoT Plugs and Nodes in large buildings, factories and farms up to 10 km from the nearest Internet access. Easy LoRa gateway uses LoRa as the transmission protocol via 433 Mhz wave. It enables us to remotely control our smart IoT Plugs and Nodes inexpensively and efficiently.
- A 10-storey building only needs 01 Easy LoRa gateway to connects to all LoRa Plugs and sensors in the building. A 10-hectare farm may only needs 01 LoRa Gateway to reach all of its IoT devices.
- Homepage: http://iotthinks.com/easy-lora-gateway/
- Full Wiki: https://github.com/IoTThinks/EasyLoRaGateway_v2/wiki

Old version of Easy LoRa gateway v1 is at https://github.com/IoTThinks/EasyLoraGateway
Old version of Easy LoRa gateway v2 is at https://github.com/IoTThinks/EasyLoRaGateway_v2

## 1. Appearance 
From top
![easy lora gateway - top](https://user-images.githubusercontent.com/29994971/50210449-bf46fb80-03a8-11e9-8eb9-035934536fb1.JPG)

From bottom
![Easy LoRa v2 1 - bottom](https://user-images.githubusercontent.com/29994971/63232975-fb43a080-c256-11e9-8285-0388fb5b3f19.JPG)

From inside (2-chip version)
![Easy LoRa v2 1 - inside](https://user-images.githubusercontent.com/29994971/63232978-fe3e9100-c256-11e9-8161-712da492a2e6.JPG)

## 2. Software feature (In progress)
- Single or dual channel LoRa gateway
- Simple local management portal
- Auto provision of Easy LoRa Plug and Node
- Forward messages from LoRa to MQTT servers
- Web Service APIs for integration
- Auto update firmware via LAN or Internet
- OPEN SOURCE and commercial version

## 3. Specification
- 1 x ESP32 chip with WiFi 802.11n and BLE
- 2 x LoRa SX1278 433MHz (1-10 km range)
- 1 x 100 Mbps Ethernet LAN
- 1 x Ethernet Console (for debug and upload firmware)
- 1 x Speaker
- 1 x LED
- 1 x Power LED
- 1 x 5v Power jack
- Over voltage protection, reverse-polarity protection, short-circuit protection via PTC fuse

## 4. Pin mappings
Pin mappings from modules to main ESP32
![image](https://user-images.githubusercontent.com/29994971/50145382-04552a00-02e4-11e9-8f4d-fa9508a38ff0.png)

## 5. Pinout of RA-01
![image](https://user-images.githubusercontent.com/29994971/60634264-8fd57900-9e38-11e9-8ed0-cd51df3bb879.png)

## 6. Pinout of ESP32
![image](https://user-images.githubusercontent.com/29994971/60634413-07a3a380-9e39-11e9-86dd-2acc5d586cae.png)

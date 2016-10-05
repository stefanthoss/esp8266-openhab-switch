# esp8266-openhab-switch
ESP8266-based switch for openHAB.

# Intro

The motivation is to build a small physical Wi-Fi enabled switch that controls lights via openHAB. It should require only a power cable and send the commands via Wi-Fi and MQTT to the openHAB server which then controls the lights.

# Hardware

Besides the openHAB server and some lights (this guide uses Philips Hue lights but others work as well), the following components are used:
* [Adafruit Feather HUZZAH with ESP8266 WiFi](https://www.adafruit.com/products/2821)
* [FeatherWing OLED - 128x32 OLED Add-on For All Feather Boards](https://www.adafruit.com/products/2900)

The OLED display is used to show network information and confirmation of the commands. The switch setup would probably also work with other ESP8266 boards but Adafruit's Feather board is nice and easy to use. The buttons on the OLED board are used as the switch buttons but it's also possible to use externally connected buttons/switches.

# ESP8266 Setup

Flash the [Arduino sketch](arduino/esp8266_openhab_switch_w_oled.ino) to the board. Don't forget to adjust the Wi-Fi and MQTT settings at the beginning of the sketch. The sketch requires that the ESP8266 drivers are installed and the Adafruit libraries for the Feather board and the OLED board are loaded in the Arduino IDE.

# openHAB Setup

## MQTT Server

On the openHAB2 server, first install the MQTT server and the openHAB MQTT binding. When using openHAB2 on a Raspberry Pi with Raspbian, the MQTT server can be installed with the following commands:

```
wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
sudo apt-key add mosquitto-repo.gpg.key
cd /etc/apt/sources.list.d/
sudo wget http://repo.mosquitto.org/debian/mosquitto-wheezy.list
sudo apt-get install mosquitto
```

After that the MQTT binding can be installed via the Paper UI. There's also a good [beginner's guide on MakeUsOf](http://www.makeuseof.com/tag/openhab-beginners-guide-part-2-zwave-mqtt-rules-charting/) that explains how to setup and test the MQTT server and binding. After installation, the MQTT binding has to be configured as shown in [services/mqtt.cfg](openhab2/services/mqtt.cfg).

## Items & Rules

In addition to the Hue light *kitchen_ceiling_light_dimmer*, two items are defined in the item file [items/all.items](openhab2/items/all.items): A switch that represents the ESP8266 switch and a string to send a message to the ESP8266 OLED display.

The rule file [rules/esp8266_switch.rules](openhab2/rules/esp8266_switch.rules) defines the switch behavior. When the ON button is pressed and the light is off, it turns on the light and sends a confirmation to the ESP8266 board. The board then displays the confirmation message for a short time.


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
Adafruit_SSD1306 display = Adafruit_SSD1306();
 
#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#else 
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif
 
#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// WiFi settings
const char* ssid        = "wifi-ssid";
const char* password    = "wifi-password";

// MQTT settings
const char* mqtt_server = "x.x.x.x";
const char* client_id   = "esp8266_switch_1";

WiFiClient espClient;
PubSubClient client(espClient);
  
void setup() {
  Serial.begin(9600);
  Serial.println("IO test");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with the I2C addr 0x3C (for the 128x32)
  Serial.println("OLED begun");
 
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.setTextColor(WHITE);
  display.setRotation(0); // Rotate the display text

  // Show screen for connecting to WiFi
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Connecting to");
  display.println("WiFi network");
  display.println(ssid);
  display.display();

  // Connecting to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }

  // Configuring MQTT server
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void show_info() {
  // Show connection success screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi & MQTT connected");
  display.println();
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Show callback screen with message received via MQTT
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(4);
  for (int i = 0; i < length; i++) {
    display.print((char)payload[i]);
  }
  display.display();
}

void reconnect() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Connecting to");
  display.println("MQTT server");
  display.println(mqtt_server);
  display.display();
      
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(client_id)) {
      client.subscribe("esp8266_switch_1/display");
      show_info();
    } else {
      delay(500);
      display.print(".");
      display.display();
    }
  }
  show_info();
}
 
void loop() {
  if (!client.connected()) {
    // Reconnect MQTT if not connected
    reconnect();
  }
  client.loop();

  // Button for setting the light to 100%
  if(!digitalRead(BUTTON_A)) {
    client.publish("esp8266_switch_1/change", "100");
    delay(500);
  }

  // Button for setting the light to 50%
  if(!digitalRead(BUTTON_B)) {
    client.publish("esp8266_switch_1/change", "50");
    delay(500);
  }

  // Button for setting the light to 0%
  if(!digitalRead(BUTTON_C)) {
    client.publish("esp8266_switch_1/change", "0");
    delay(500);
  }
}

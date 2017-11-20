/* UserSpice MQTT Demo Sketch
If you get errors using this sketch, go to your arduino software and go to
Sketch->Include Library->Manage Libraries
In the search box, find and install
PubSubClient
NeoPixel by Adafruit
DallasTemperature by Miles Burton et al
Then restart your arduino software.

Note that this version is expecting an Arduino Uno/Mega with a 5100 Ethernet Shield
but the code will be similar for the ESP8266 and ESP32. If enough people request it,
I'll do a demo for those as well.
*/


#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <DallasTemperature.h>

const int relay1 = 6;
const int relay2 = 7;
#define PIN 8 //neopixel ring
#define ONE_WIRE_BUS 9 //Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS); //Setup OneWire Bus
DallasTemperature sensors(&oneWire);


int request = 0;
long lastMsg = 0;
float temp = 0;
char *cstring;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);



// Update these with values suitable for your network.
byte mac[]    = {  0x28, 0xFF, 0xA2, 0xBC, 0xB5, 0x16, 0x03, 0x56 };
IPAddress ip(192, 168, 95, 239);
IPAddress server(192, 168, 95, 222);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();



  if (strstr(topic, "relay") != NULL) {
    Serial.println(request);
    if (strstr(topic, "relay1on") != NULL) {
      digitalWrite(relay1, LOW); //on
    }
    if (strstr(topic, "relay1off") != NULL) {
      digitalWrite(relay1, HIGH); //off
    }
    if (strstr(topic, "relay2on") != NULL) {
      digitalWrite(relay2, LOW); //on
    }
    if (strstr(topic, "relay2off") != NULL) {
      digitalWrite(relay2, HIGH); //off
    }
}

if (strstr(topic, "ring") != NULL) {
  if (strstr(payload, "red") != NULL) {
      colorWipe(strip.Color(255, 0, 0), 35);
      colorWipe(strip.Color(0, 0, 0), 35);
    }
  if (strstr(payload, "green") != NULL) {
      colorWipe(strip.Color(0, 255, 0), 35);
      colorWipe(strip.Color(0, 0, 0), 35);
    }
  if (strstr(payload, "blue") != NULL) {
      colorWipe(strip.Color(0, 0, 255), 35);
      colorWipe(strip.Color(0, 0, 0), 35);
    }
  if (strstr(payload, "rainbow") != NULL) {
    rainbowCycle(1);
    colorWipe(strip.Color(0, 0, 0), 22);
  }

}

}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("arduino","hello world");

      // ... and resubscribe
      client.subscribe("usIN/#");
      // client.subscribe('input');
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//these are some effect loops for the led ring
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setup()
{
//Setting up the arduino pins
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  Serial.begin(57600);

//setting up the neopixel led ring
strip.begin();
strip.setBrightness(30); //adjust brightness here
strip.show(); // Initialize all pixels to 'off'

//start temperature sensors
 sensors.begin();

//preparing the Ethernet/MQTT stuff
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
if (now - lastMsg > 60000) {
  lastMsg = now;
  sensors.setResolution(10);
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp = sensors.getTempFByIndex(0); //Change to getTempC for Celcius
  Serial.println(temp);

    client.publish("arduino/temp", String(temp).c_str(),TRUE);

  }
}

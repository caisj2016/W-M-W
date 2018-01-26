#include <ESP8266WiFi.h>
#include <Milkcocoa.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Test001"
#define WLAN_PASS       "hashimoto"
/************************* Your Milkcocoa Setup *********************************/

#define MILKCOCOA_APP_ID      "vuejb91il2k"
#define MILKCOCOA_DATASTORE   "twitter-mode-red"

/************* Milkcocoa Setup (you don't need to change this!) ******************/

#define MILKCOCOA_SERVERPORT  1883

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);
//LED
const int ledPin =  12;
int ledState = LOW;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) );
  digitalWrite(ledPin, ledState);
};

void loop() {
  milkcocoa.loop();
  delay(200);
};

void onpush(DataElement *elem) {
  for (int i = 0; i < 6; i ++)
  {
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledPin, ledState);
    delay(200);
  }
};


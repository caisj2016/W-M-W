#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

//https://maker.ifttt.com/trigger/action_1/with/key/coTMYJkEfMIRnRXE2A56_y
// Wi-FiアクセスポイントのSSIDとパスワード
const char* ssid = "Test001";
const char* password = "hashimoto";

// IFTTTのシークレットキー
const char* key = "osF2ut62VKiY3zUuSEOyW";

const char* host = "maker.ifttt.com";
const char* event   = "action_1";
const int httpsPort = 443;

WiFiClientSecure client;
int i;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  i++;
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // URLを作成
  // maker.ifttt.com/trigger/{event}/with/key/{key}
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += key;

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& dat = jsonBuffer.createObject();
  dat["value1"] =  "test"+i;
  String value;
  dat.printTo(value);
  value += "\r\n";
  int contentlength = value.length();
  // ウェブリクエストを送信
  String httpPacket = "POST " + url + " HTTP/1.1\r\nHost: " + host + "\r\nContent-Length: " + contentlength + " \r\nContent-Type: application/json\r\n\r\n" + value + "\r\n";
  int length = httpPacket.length();

  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10);

  // Send our http request
  Serial.print(httpPacket);
  client.print(httpPacket);
  delay(40);
  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
    Serial.println("closing connection");
  }
  delay(60000);
}


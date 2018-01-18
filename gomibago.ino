#include <ESP8266WiFi.h>
#include <Milkcocoa.h>
#include <SoftwareSerial.h>

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
//人感センサー
int pir = 2;
//モーター
int A_IN1 = 3;
int A_IN2 = 4;
int B_IN1 = 5;
int B_IN2 = 6;
// int VR_PIN = A0;
// int value = 500; //0~255の数値を入れてください。速度回転方向が変化します。
// int turn_value = 50; //0~255の数値を入れてください。速度回転方向が変化します。(曲がる時)

//障害物センサー ※番号まだ決めてない
int buttonpin = 7;

//発話認識
#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);

bool talkFlg  = false;

const char *voiceBuffer[] =
{
    "Turn on the light",
    "Turn off the light",
    "Play music",
    "Pause",
    "Next",
    "Previous",
    "Up",
    "Down",
    "Turn on the TV",
    "Turn off the TV",
    "Increase temperature",
    "Decrease temperature",
    "What's the time",
    "Open the door",
    "Close the door",
    "Left",
    "Right",
    "Stop",
    "Start",
    "Mode 1",
    "Mode 2",
    "Go",
};

void setup() {
  Serial.begin(115200);
  delay(10);

  softSerial.begin(9600);
  softSerial.listen();

  pinMode(ledPin, OUTPUT);
  pinMode(A_IN1,OUTPUT);
  pinMode(A_IN2,OUTPUT);
  pinMode(B_IN1,OUTPUT);
  pinMode(B_IN2,OUTPUT);

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
  char cmd;
  if(getCmd()==9){
    !talkFlg;
  }
  //モード4
  if(talkFlg){
    cmd=getCmd()
    //19 Start 
    //18 Stop
    //17 Right
    //16 Left
    switch(cmd)
    {
    	//TODO その他動作
        case 19: foward();break;
        case 18: despin();break;
        case 17: foward_right();break;
        case 16: foward_left();break;
        default:break;
    }
    delay(500);
  }
  //モード3 人感センサー
  if(digitalRead(pir)) {
    //（バック）
    reverse();
    delay(500);
  }else{
    //止める
    despin();
  }

  val=digitalRead(buttonpin); 

  //モード2 障害物センサー
  if(val==HIGH) {
    //動作a
    //TODO
  }else{
    //動作aを止める
	//TODO
  }
  //モード1　onpushの動作確認
  milkcocoa.loop();
  delay(200);
};

void onpush(DataElement *elem) {
  for (int i = 0; i < 6; i ++)
  {
  //TODO 今LEDを開けだけ
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledPin, ledState);
    delay(200);
  }
};

// 順方向に回転(前進)
void foward(){
      digitalWrite(A_IN1, HIGH);
      digitalWrite(A_IN2, LOW);
      digitalWrite(B_IN1, HIGH);
      digitalWrite(B_IN2, LOW);
}

// 逆方向に順回転（バック）
void reverse(){
      digitalWrite(A_IN1, LOW);
      digitalWrite(A_IN2, HIGH);
      digitalWrite(B_IN1, LOW);
      digitalWrite(B_IN2, HIGH);
};

// 右方向に順回転
void foward_right(){
      digitalWrite(A_IN1, HIGH);
      digitalWrite(A_IN2, LOW);
      digitalWrite(B_IN1, LOW);
      digitalWrite(B_IN2, LOW); 
};

// 左方向に順回転
void foward_left(){
      digitalWrite(A_IN1, LOW);
      digitalWrite(A_IN2, LOW);
      digitalWrite(B_IN1, HIGH);
      digitalWrite(B_IN2, LOW); 
};

// 右方向に逆回転
void reverse_right(){
      digitalWrite(A_IN1, LOW);
      digitalWrite(A_IN2, HIGH);
      digitalWrite(B_IN1, LOW);
      digitalWrite(B_IN2, LOW); 
};

// 左方向に逆回転
void reverse_left(){
      digitalWrite(A_IN1, LOW);
      digitalWrite(A_IN2, LOW);
      digitalWrite(B_IN1, LOW);
      digitalWrite(B_IN2, HIGH); 
  };

// 回転を停止
void despin(){
      digitalWrite(A_IN1, HIGH);
      digitalWrite(A_IN2, HIGH);
      digitalWrite(B_IN1, HIGH);
      digitalWrite(B_IN2, HIGH);  
};

//cmd値を取得
char getCmd(){
    if(softSerial.available()){
      cmd = softSerial.read();
      return cmd;
    }  
}


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "MP3.h"

//人感センサー
#define PIR_MOTION_SENSOR 2 
//モーター
const int A_IN1 = 3;
const int A_IN2 = 4;
const int B_IN1 = 5;
const int B_IN2 = 6;

//障害物センサー ※番号まだ決めてない
int buttonpin = 7;

const boolean talkFlg=false;
// int VR_PIN = A0;
// int value = 500; //0~255の数値を入れてください。速度回転方向が変化します。
// int turn_value = 50; //0~255の数値を入れてください。速度回転方向が変化します。(曲がる時)

#define SOFTSERIAL_RX_PIN  4
#define SOFTSERIAL_TX_PIN  5
SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);
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
//音楽　
SoftwareSerial mp3Serial(3, 4); // RX, TX
MP3 player(mp3Serial);

void setup() {
  Serial.begin(9600); 
  softSerial.listen();
  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(A_IN1,OUTPUT);
  pinMode(A_IN2,OUTPUT);
  pinMode(B_IN1,OUTPUT);
  pinMode(B_IN2,OUTPUT);
}



void loop() {
    if(softSerial.available()){
        if(softSerial.read()==9){
            !talkFlg;
        }
    }

    //モード2 障害物センサー
    if(digitalRead(buttonpin)==HIGH) {
        player.playSong(1);
        delay(5000);
        player.stop();
    }

    //モード3 人感センサー
    if(isPeopleDetected()){
        Serial.println("***************"); 
          player.playSong(2);
         delay(5000);
        } else {
        Serial.println("$$$$$$$$$$$$$$$$$"); 
        player.stop();
        delay(5000);
       
      }
      //モード4
    if(talkFlg){
        //19 Start 
        //18 Stop
        //17 Right
        //16 Left
    switch(softSerial.read())
    {
        case 19: foward();break;
        case 18: despin();break;
        case 17: foward_right();break;
        case 16: foward_left();break;
        //TODO
        default:break;
    }
        delay(500);
    }
}
//人感センサー
boolean isPeopleDetected()
{
    int sensorValue = digitalRead(PIR_MOTION_SENSOR);
    if(sensorValue == HIGH) {
        return true;
    } else {
        return false;
    }
}
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
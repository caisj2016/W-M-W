#include <Arduino.h>
#include <SoftwareSerial.h>
#include "MP3.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
//人感センサー
#define PIR_MOTION_SENSOR 8
//モーター
const int A_IN1 = 9;
const int A_IN2 = 10;
const int B_IN1 = 11;
const int B_IN2 = 12;

//障害物センサー ※番号まだ決めてない
int buttonpin = 4;

boolean talkFlg = false;
// int VR_PIN = A0;
// int value = 500; //0~255の数値を入れてください。速度回転方向が変化します。
// int turn_value = 50; //0~255の数値を入れてください。速度回転方向が変化します。(曲がる時)

#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3
SoftwareSerial softSerial(SOFTSERIAL_RX_PIN, SOFTSERIAL_TX_PIN);
char cmd;
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
SoftwareSerial mp3Serial(6, 7); // RX, TX
MP3 player(mp3Serial);
#define PIN 7 // NeoPixelピン

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GBR + NEO_KHZ800);

int gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(A_IN1, OUTPUT);
  pinMode(A_IN2, OUTPUT);
  pinMode(B_IN1, OUTPUT);
  pinMode(B_IN2, OUTPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



void loop() {
  //モード2 障害物センサー
  if (digitalRead(buttonpin) == HIGH) {
    Serial.println("33333333333");
    //音楽用意必要
    player.playSong(5);
    strip.setBrightness(200);
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    colorWipe(strip.Color(0, 0, 0, 255), 50); // White
    whiteOverRainbow(20, 75, 5);
    pulseWhite(5);
    //回す　動作動作必要
    mota(1);
    delay(5000);
  }

  //モード3 人感センサー
  if (isPeopleDetected()) {
    Serial.println("222222222222");

    //逃げる動き　動作必要
    mota(2);
  } 
  //モード4
  //3 Music
  //4 pasuse
  //5 Next
  //6 Previous
  //13 What’s the time
  //20 Mode 1
  //19 Start
  //18 Stop
  //17 Right
  //16 Left
  if(softSerial.available())
  {
    cmd = softSerial.read();
    switch (cmd)
    {
      //音楽用意必要
      case 3: player.playSong(1);break;
      case 4: player.pausePlay(); break;
      case 5: player.next(); break;
      case 6: player.prev(); break;
      //想像外、変な動き　動作必要
      case 13: mota(3); break;
      case 20: mota(4); break;
      case 19: foward(); break;
      case 18: despin(); break;
      case 17: foward_right(); break;
      case 16: foward_left(); break;
      //TODO
      default: break;
        delay(500);
    }
      Serial.println(voiceBuffer[cmd - 1]);
  } 
}
//人感センサー
boolean isPeopleDetected()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if (sensorValue == HIGH) {
    return true;
  } else {
    return false;
  }
}
//障害物センサー
boolean isThingsDetected()
{
  int sensorValue = digitalRead(buttonpin);
  if (sensorValue == HIGH) {
    return true;
  } else {
    return false;
  }
}
// 順方向に回転(前進)
void foward() {
  digitalWrite(A_IN1, HIGH);
  digitalWrite(A_IN2, LOW);
  digitalWrite(B_IN1, HIGH);
  digitalWrite(B_IN2, LOW);
}

// 逆方向に順回転（バック）
void reverse() {
  digitalWrite(A_IN1, LOW);
  digitalWrite(A_IN2, HIGH);
  digitalWrite(B_IN1, LOW);
  digitalWrite(B_IN2, HIGH);
};

// 右方向に順回転
void foward_right() {
  digitalWrite(A_IN1, HIGH);
  digitalWrite(A_IN2, LOW);
  digitalWrite(B_IN1, LOW);
  digitalWrite(B_IN2, LOW);
};

// 左方向に順回転
void foward_left() {
  digitalWrite(A_IN1, LOW);
  digitalWrite(A_IN2, LOW);
  digitalWrite(B_IN1, HIGH);
  digitalWrite(B_IN2, LOW);
};

// 右方向に逆回転
void reverse_right() {
  digitalWrite(A_IN1, LOW);
  digitalWrite(A_IN2, HIGH);
  digitalWrite(B_IN1, LOW);
  digitalWrite(B_IN2, LOW);
};

// 左方向に逆回転
void reverse_left() {
  digitalWrite(A_IN1, LOW);
  digitalWrite(A_IN2, LOW);
  digitalWrite(B_IN1, LOW);
  digitalWrite(B_IN2, HIGH);
};

// 回転を停止
void despin() {
  digitalWrite(A_IN1, HIGH);
  digitalWrite(A_IN2, HIGH);
  digitalWrite(B_IN1, HIGH);
  digitalWrite(B_IN2, HIGH);
};

//mota動作　動作確認必要
//モーターの動作　勝手に設定された、動作変更をお願いします
void mota(int m){
    switch(m){
        case '1':
            {
                Serial.println("11111111111");
                foward();
                delay(500);
                reverse();
                delay(500);
                despin();
            }
            break;
        case '2':
            {
                Serial.println("222222222222");
                foward_left();
                delay(500);
                reverse();
                delay(500);
                despin();
            }
            break;
        case '3':
            {
                Serial.println("33333333333");
                foward_left();
                delay(500);
                foward();
                delay(500);
                despin();
            }
            break;
        case '4':
            {
                Serial.println("4444444444");
                reverse_left();
                delay(500);
                foward_right();
                delay(500);
                despin();
            }
            break;
        default:
            Serial.println("Can not find a valid action");     
        }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  for (int j = 0; j < 256 ; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0, gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }

  for (int j = 255; j >= 0 ; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0, gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }
}

void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for (int k = 0 ; k < rainbowLoops ; k ++) {

    for (int j = 0; j < 256; j++) { // 5 cycles of all colors on wheel

      for (int i = 0; i < strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal / fadeMax);
        greenVal = green(wheelVal) * float(fadeVal / fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal / fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if (k == 0 && fadeVal < fadeMax - 1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if (k == rainbowLoops - 1 && j > 255 - fadeMax ) {
        fadeVal--;
      }

      strip.show();
      delay(wait);
    }

  }



  delay(500);


  for (int k = 0 ; k < whiteLoops ; k ++) {

    for (int j = 0; j < 256 ; j++) {

      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, gamma[j] ) );
      }
      strip.show();
    }

    delay(2000);
    for (int j = 255; j >= 0 ; j--) {

      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, gamma[j] ) );
      }
      strip.show();
    }
  }

  delay(500);


}




void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {

  if (whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while (true) {
    for (int j = 0; j < 256; j++) {
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ) {
          strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
        }
        else {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }

      }

      if (millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if (head == strip.numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if (loopNum == loops) return;

      head %= strip.numPixels();
      tail %= strip.numPixels();
      strip.show();
      delay(wait);
    }
  }

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}
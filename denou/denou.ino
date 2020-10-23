//--------------------------------------------------------------------------------
// 電王変身ベルト
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
//
// https://twitter.com/masashi_214
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------

#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

#define RedSw 12
#define BlueSw 14
#define YellowSw 15
#define PurpleSw 16

SoftwareSerial mySoftwareSerial(A4, A5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//Task Schedule
unsigned long gPrevious = 0;

void setup() {
  Serial.begin(115200);

  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
        
  pinMode(RedSw, INPUT_PULLUP);   // プルアップでポート設定
  pinMode(BlueSw, INPUT_PULLUP);
  pinMode(YellowSw, INPUT_PULLUP);
  pinMode(PurpleSw, INPUT_PULLUP);

  //Reset task
  gPrevious = millis();
}

void loop() {
  if( (millis() - gPrevious) >= 100){  // 100msec scan
    StateMachine();
    //Reset task
    gPrevious = millis();
  }
}

void StateMachine(void)
{
  enum{
      ST_SWSCAN = 0,
      ST_MP3PLAY,
  };
  
  static int state = ST_SWSCAN;
  static int SoundNo = 0;
  
  switch(state){
    case ST_SWSCAN:// スイッチスキャンステート
                    if(digitalRead(RedSw) == 0){
                      Serial.println("RedSw");
                      SoundNo = 1;
                      state = ST_MP3PLAY;
                      break;
                    }                    
                    if(digitalRead(BlueSw) == 0){
                      Serial.println("BlueSw");
                      SoundNo = 2;
                      state = ST_MP3PLAY;
                      break;
                    }
                    if(digitalRead(YellowSw) == 0){
                      Serial.println("YellowSw");
                      SoundNo = 3;
                      state = ST_MP3PLAY;
                      break;
                    }
                    if(digitalRead(PurpleSw) == 0){
                      Serial.println("PurpleSw");
                      SoundNo = 4;
                      state = ST_MP3PLAY;
                      break;
                    }
                    break;
                    
    case ST_MP3PLAY:// MP3 Sound 再生ステート
                    myDFPlayer.play(SoundNo);
                    Serial.print("SoundNo:");
                    Serial.println(SoundNo);
                    state = ST_SWSCAN;
                    break;
                    
    default:
                    break;
  }
  
}

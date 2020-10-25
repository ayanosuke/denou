//--------------------------------------------------------------------------------
// 電王変身ベルト
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
//
// https://twitter.com/masashi_214
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------


#include "Adafruit_NeoPixel.h"
#include "DFRobotDFPlayerMini.h"
#include "SeqLight.h"
#include <SoftwareSerial.h>


#define RedSw 12
#define BlueSw 14
#define YellowSw 15
#define PurpleSw 16

SoftwareSerial mySoftwareSerial(A4, A5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
Adafruit_NeoPixel pixels(2, 3, NEO_GRB + NEO_KHZ800);

  SeqLight Led = SeqLight(1);
//  SeqLight LedB = SeqLight(2);
//  SeqLight LedC = SeqLight(3);
//  SeqLight LedD = SeqLight(3);

//Task Schedule
unsigned long gPrevious = 0;
unsigned long gPreviousL = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("DENOU BELT");
  
  pixels.begin();
  Led.Select(1);
  Led.stateCheck(); 
  
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(15);  //Set volume value. From 0 to 30 10
        
  pinMode(RedSw, INPUT_PULLUP);     // プルアップでポート設定
  pinMode(BlueSw, INPUT_PULLUP);
  pinMode(YellowSw, INPUT_PULLUP);
  pinMode(PurpleSw, INPUT_PULLUP);
  pinMode(4,INPUT);                 // リードスイッチ
  //Reset task
  
  gPrevious = gPreviousL = millis();
}

void loop() {
  if( (millis() - gPrevious) >= 100){  // 100msec scan
    StateMachine();
    //Reset task
    gPrevious = millis();
  }

  if( (millis() - gPreviousL) >= 10){  // 10msec scan
    Led.stateCheck();
    //Reset task
    gPreviousL = millis();
  }

}

void StateMachine(void)
{
  enum{
      ST_HENSHINTAIKI = 0,
      ST_HENSHIN,
      ST_HENSHINTYU,
      ST_FULL,
      ST_SWSCAN ,
      ST_MP3PLAY,
  };
  
  static int MainState = ST_HENSHINTAIKI;
  static int SoundNo = 0;
  
  switch(MainState){
    case ST_HENSHINTAIKI:// 変身待機
                        Serial.println("ST_HENSHINTAIKI:");
//                      myDFPlayer.play(5);
                        myDFPlayer.loop(5); // mp3の間は途切れますね
                        Led.OnOff(ON);
                        MainState = ST_SWSCAN;
                        break;
                        
    case ST_SWSCAN:     // スイッチスキャンステート
                        Serial.println("ST_SWSCAN:");
                        if(digitalRead(4) == 0){      // 磁石検出
                          Led.OnOff(OFF);
                          MainState = ST_HENSHIN;
                          break;
                        }
                        if(digitalRead(RedSw) == 0){
                          Serial.println("RedSw");
                          SoundNo = 1;
                          MainState = ST_MP3PLAY;
                          break;
                        }                    
                        if(digitalRead(BlueSw) == 0){
                          Serial.println("BlueSw");
                          SoundNo = 2;
                          MainState = ST_MP3PLAY;
                          break;
                        }
                        if(digitalRead(YellowSw) == 0){
                          Serial.println("YellowSw");
                          SoundNo = 3;
                          MainState = ST_MP3PLAY;
                          break;
                        }
                        if(digitalRead(PurpleSw) == 0){
                          Serial.println("PurpleSw");
                          SoundNo = 4;
                          MainState = ST_MP3PLAY;
                          break;
                        }
                        break;

    case ST_HENSHIN:     //カードかざして変身
                        Led.Select(2);
                        Led.OnOff(ON);
                        myDFPlayer.play(6);
                        MainState = ST_HENSHINTYU; 
                        break;

    case ST_HENSHINTYU:     //変身中
                        if(digitalRead(4) == 0){      // 磁石検出
                          Led.OnOff(OFF);
                          MainState = ST_FULL;
                          break;
                        }
                        
                        break;
    case ST_FULL:
                        Led.Select(3);
                        Led.OnOff(ON);
                        myDFPlayer.play(7);
                        MainState = ST_SWSCAN;
                        break;
                        
    case ST_MP3PLAY:// MP3 Sound 再生ステート
                        Serial.println("ST_MP3PLAY");
                    myDFPlayer.play(SoundNo);
                    Serial.print("SoundNo:");
                    Serial.println(SoundNo);
                    MainState = ST_SWSCAN;
                    break;
                    
    default:
                    break;
  }
  
}






//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [SeqLight.h]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------

// コンストラクタ
SeqLight::SeqLight( unsigned char mode)
{
  state = ST_INIT;
  
  switch(mode){
    case 0: ptn = ptn0;
            break;
    case 1: ptn = ptn1;
            break;
    case 2: ptn = ptn2;
            break;
    case 3: ptn = ptn3;    
            break;
    case 4: ptn = ptn4;
            break;
    case 5: ptn = ptn3;
            break;
    default:
            break;
   }

}  

void SeqLight::Select( unsigned char mode )
{
  switch(mode){
    case 0: ptn = ptn0;
            break;
    case 1: ptn = ptn1;
            break;
    case 2: ptn = ptn2;
            break;
    case 3: ptn = ptn3;    
            break;
    case 4: ptn = ptn4;
            break;
    case 5: ptn = ptn3;
            break;
    default:
            break;
   }
}  


void SeqLight::OnOff(unsigned char sw)
{
  if( sw == 0) {
    state = ST_OFF;
  } else if( sw == 1) {
    state = ST_ON;
  }
  Serial.print("OnOff:");
  Serial.print(sw);      
  Serial.print(",");
  Serial.println(state);      

}

//---------------------------------------------------------------------
// FX効果ステートマシン
// 10ms周期で起動
//---------------------------------------------------------------------
void SeqLight::stateCheck()
{
  switch(state){
    case ST_INIT:// 0
//              Serial.println("ST_INIT:");   
                pixels.setPixelColor(0, pixels.Color(0,0,0));
                pixels.setPixelColor(1, pixels.Color(0,0,0));
                pixels.show();
                state = ST_IDLE;
                break;
    case ST_IDLE:// 1
//              Serial.println("ST_IDLE:");   
                state = ST_IDLE;
                break;
    case ST_ON: // 2
//               Serial.println("ST_ON:");   
                adr = 0;

                LedMainR.LedRef = 0;
                LedMainG.LedRef = 0;
                LedMainB.LedRef = 0;
                

                pixels.setPixelColor(0, pixels.Color(0,0,0));
                pixels.setPixelColor(1, pixels.Color(0,0,0));
                pixels.show();          
                state = ST_FARST;
                break;
    case ST_OFF: // 3
//              Serial.println("ST_OFF:");   
                adr = 0;

                LedMainR.LedRef = 0;
                LedMainG.LedRef = 0;
                LedMainB.LedRef = 0;
    
                pixels.setPixelColor(0, pixels.Color(0,0,0));          
                pixels.setPixelColor(1, pixels.Color(0,0,0));
                pixels.show();
                state = ST_IDLE;
                break;
 
    case ST_FARST: //4
//              Serial.println("ST_4:");   

                LedMainR.LedRef = ptn[adr][2];
                LedMainG.LedRef = ptn[adr][3];
                LedMainB.LedRef = ptn[adr][4];
                LedSel  = ptn[adr][5];
#if 0           
Serial.print("ADR:");
Serial.print(adr);
Serial.print(",Ref:");
Serial.print(LedMainR.LedRef);
Serial.print(",");
Serial.print(LedMainG.LedRef);
Serial.print(",");
Serial.println(LedMainB.LedRef);
Serial.println("00");  
#endif
                switch(LedSel){
                  case 0:
                          pixels.setPixelColor(0, pixels.Color(ptn[adr][2],ptn[adr][3],ptn[adr][4])); 
                          pixels.setPixelColor(1, pixels.Color(0,0,0)); 
                          break;
                  case 1:
                          pixels.setPixelColor(0, pixels.Color(0,0,0)); 
                          pixels.setPixelColor(1, pixels.Color(ptn[adr][2],ptn[adr][3],ptn[adr][4]));
                          break;
                  case 2:
                          pixels.setPixelColor(0, pixels.Color((int)LedMainR.LedRef,(int)LedMainG.LedRef,(int)LedMainB.LedRef));          
                          pixels.setPixelColor(1, pixels.Color((int)LedMainR.LedRef,(int)LedMainG.LedRef,(int)LedMainB.LedRef));   
                          break;
                  default:
                          break;
                }
                pixels.show();
                
                styTime = ptn[adr+1][1];
                
                LedMainR.nowPwm = LedMainR.LedRef;
                LedMainG.nowPwm = LedMainG.LedRef;
                LedMainB.nowPwm = LedMainB.LedRef;                

                LedMainR.nextPwm = ptn[adr+1][2];
                LedMainG.nextPwm = ptn[adr+1][3];
                LedMainB.nextPwm = ptn[adr+1][4];  
#if 0
Serial.print("ADR:");
Serial.print(adr);
Serial.print(",tim:");
Serial.print(styTime);
Serial.print(",Ref:");
Serial.print(LedMainR.nextPwm);
Serial.print(",");
Serial.print(LedMainG.nextPwm);
Serial.print(",");
Serial.println(LedMainB.nextPwm);
#endif                           
                if(styTime == 0 || ptn[adr+1][0]=='O' ){
                  LedMainR.deltPwm = LedMainR.nextPwm - LedMainR.nowPwm;
                  LedMainG.deltPwm = LedMainG.nextPwm - LedMainG.nowPwm;
                  LedMainB.deltPwm = LedMainB.nextPwm - LedMainB.nowPwm;   
                } else {        
                  LedMainR.deltPwm = ( LedMainR.nextPwm - LedMainR.nowPwm ) /  (float)styTime;
                  LedMainG.deltPwm = ( LedMainG.nextPwm - LedMainG.nowPwm ) /  (float)styTime;
                  LedMainB.deltPwm = ( LedMainB.nextPwm - LedMainB.nowPwm ) /  (float)styTime;   
                }
#if 0
Serial.print("deltPwm:");
Serial.print(LedMainR.deltPwm);
Serial.print(",");
Serial.print(LedMainG.deltPwm);
Serial.print(",");
Serial.println(LedMainB.deltPwm);
#endif
         
                if((LedMainR.nextPwm - LedMainR.nowPwm == 0) && (LedMainG.nextPwm - LedMainG.nowPwm == 0) && (LedMainB.nextPwm - LedMainB.nowPwm == 0)){  // 赤LEDを代表してチェック
                  updownFlg = STY;
                  state = ST_STAY;
                  break;
                } else if(LedMainR.nextPwm - LedMainR.nowPwm < 0)  // 赤LEDを代表してチェック
                  updownFlg = DOWN;
                else {
                  updownFlg = UP;
                }
                if(ptn[adr+1][0]=='O') {
                  state = ST_STAY;                
                } else {
                  state = ST_RUN;
                }
                break;
                
    case ST_STAY://5
//                Serial.println("ST_5:");   
                  styTime --;
                  if(styTime <= 0)
                    state = ST_NEXT;
                  break;

    case ST_RUN://6
//                    Serial.print("ST_6:");   
//                    Serial.print("LedSel:");  
//                    Serial.println((int)LedSel);  
                  LedMainR.LedRef = LedMainR.LedRef + LedMainR.deltPwm;
                  LedMainG.LedRef = LedMainG.LedRef + LedMainG.deltPwm;
                  LedMainB.LedRef = LedMainB.LedRef + LedMainB.deltPwm;   

                  if((updownFlg == DOWN) && (LedMainR.LedRef <= LedMainR.nextPwm) && (LedMainG.LedRef <= LedMainG.nextPwm) && (LedMainB.LedRef <= LedMainB.nextPwm)){ // 赤LEDを代表してチェック
                    pixels.setPixelColor(0, pixels.Color((int)LedMainR.nextPwm,(int)LedMainG.nextPwm,(int)LedMainB.nextPwm));          
                    pixels.setPixelColor(1, pixels.Color((int)LedMainR.nextPwm,(int)LedMainG.nextPwm,(int)LedMainB.nextPwm));   
                    pixels.show();
                    state = ST_NEXT;
                  } else if((updownFlg == UP) && (LedMainR.LedRef >= LedMainR.nextPwm)&& (LedMainG.LedRef >= LedMainG.nextPwm)&& (LedMainB.LedRef >= LedMainB.nextPwm)){ // 赤LEDを代表してチェック
                switch(LedSel){
                  case 0:
                          pixels.setPixelColor(0, pixels.Color(ptn[adr][2],ptn[adr][3],ptn[adr][4])); 
                          pixels.setPixelColor(1, pixels.Color(0,0,0)); 
                          break;
                  case 1:
                          pixels.setPixelColor(0, pixels.Color(0,0,0)); 
                          pixels.setPixelColor(1, pixels.Color(ptn[adr][2],ptn[adr][3],ptn[adr][4]));  
                          break;
                  case 2:
                          pixels.setPixelColor(0, pixels.Color((int)LedMainR.nextPwm,(int)LedMainG.nextPwm,(int)LedMainB.nextPwm));          
                          pixels.setPixelColor(1, pixels.Color((int)LedMainR.nextPwm,(int)LedMainG.nextPwm,(int)LedMainB.nextPwm));   
                          break;
                  default:
                          break;
                }
                    pixels.show();
                    LedMainR.LedRef = LedMainR.nextPwm;
                    LedMainG.LedRef = LedMainG.nextPwm;
                    LedMainB.LedRef = LedMainB.nextPwm;   
                    state = ST_NEXT;
                  } else {
                    pixels.setPixelColor(0, pixels.Color((int)LedMainR.LedRef,(int)LedMainG.LedRef,(int)LedMainB.LedRef));          
                    pixels.setPixelColor(1, pixels.Color((int)LedMainR.LedRef,(int)LedMainG.LedRef,(int)LedMainB.LedRef));   
                    pixels.show();
                  }
                  if(ptn[adr][0]=='O')
                    state = ST_STAY;
                  break;
    case ST_NEXT://7
//                  Serial.print("ST_7:");   
                  adr++;
                  
                  if(ptn[adr][0]=='E'){
                    state = ST_IDLE;
                    break;
                  }
                  if(ptn[adr][0]=='L'){
                    adr = 0;
                    state = ST_FARST;
                    break;
                  }
                  
//                nowPwm = ptn[adr-1][2];
//                nextPwm = ptn[adr][2];
                  styTime = ptn[adr][1];
                  LedSel  = ptn[adr][5];
 //                   Serial.print("LedSel:");  
  //                  Serial.println((int)LedSel);                  
                  LedMainR.nowPwm = ptn[adr-1][2];
                  LedMainG.nowPwm = ptn[adr-1][3];
                  LedMainB.nowPwm = ptn[adr-1][4];                

                  LedMainR.nextPwm = ptn[adr][2];
                  LedMainG.nextPwm = ptn[adr][3];
                  LedMainB.nextPwm = ptn[adr][4];  


                if(styTime == 0|| ptn[adr][0]=='O'){
                  LedMainR.deltPwm = LedMainR.nextPwm - LedMainR.nowPwm;
                  LedMainG.deltPwm = LedMainG.nextPwm - LedMainG.nowPwm;
                  LedMainB.deltPwm = LedMainB.nextPwm - LedMainB.nowPwm;   
                } else {
                  LedMainR.deltPwm = ( LedMainR.nextPwm - LedMainR.nowPwm ) / (float)styTime;
                  LedMainG.deltPwm = ( LedMainG.nextPwm - LedMainG.nowPwm ) / (float)styTime;
                  LedMainB.deltPwm = ( LedMainB.nextPwm - LedMainB.nowPwm ) / (float)styTime;   
                }
                  if(LedSel == 2 && (LedMainR.nextPwm - LedMainR.nowPwm == 0) &&(LedMainG.nextPwm - LedMainG.nowPwm == 0) && (LedMainB.nextPwm - LedMainB.nowPwm == 0) ){  // 赤LEDを代表してチェック
                    updownFlg = STY;
                    styTime = ptn[adr][1];
                    state = ST_STAY;
                    break;
                  } else if((LedMainR.nextPwm - LedMainR.nowPwm < 0) || (LedMainG.nextPwm - LedMainG.nowPwm < 0) ||(LedMainB.nextPwm - LedMainB.nowPwm < 0))  // 赤LEDを代表してチェック
                    updownFlg = DOWN;
                  else {
                    updownFlg = UP;
                  }
                  state = ST_RUN;
                  break;
    default:
//                  Serial.println("default");   
                  break;
  }
}

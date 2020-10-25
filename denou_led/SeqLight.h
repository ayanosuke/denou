//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [SeqLight.h]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------
  enum{
    FastOn = 1,
    SlowOn,
    TriangleWave,
    Random,
    MarsLight,
    FlashLight,
    SinglePulesStrobe,
    DoublePulseStrobe,
    MediumPulseStrobo,
    FluorescentLamp,
    BrokenFluorescentLamp,
  };

  enum{
        OFF = 0,
        ON,
  };



class SeqLight
{
public:
  SeqLight(unsigned char mode);
  void Select(unsigned char mode);
  void stateCheck();
  void OnOff(unsigned char sw);
  
private:
  typedef struct {
    float LedRef;
    float deltPwm;
    int nowPwm;
    int nextPwm;

 } Led_t;

  Led_t LedMainR;
  Led_t LedMainG;
  Led_t LedMainB;
  
  int state;// = ST_IDLE;
  unsigned char port;
  unsigned char mode;
  char updownFlg;

  unsigned char adr = 0;      // アドレス
  
  float rRef =0;
  float gRef =0;
  float bRef =0;

  float deltPwm;            // 10msあたりのpwm増加量
  
  int nowPwm;
  int nextPwm;
  int styTime;
  char LedSel;

  unsigned char (*ptn)[6];
  
  enum{
    ST_INIT = 0,
    ST_IDLE,
    ST_ON,
    ST_OFF,
    ST_FARST,
    ST_STAY,
    ST_RUN,
    ST_NEXT,
  };

  enum{
    DOWN = 0,
    STY,
    UP,
  };


//Cmd,Time,R,G,B
//I:初期状態,O:出力,S:スイープ,L:ループ,E:終了

const unsigned char ptn0[4][6]={
                          {'I',  0,   0,   0,   0, 2},
                          {'E',  0,   0,   0,   0, 2}};
                          
const unsigned char ptn1[4][6]={
                          {'I',  0,   0,   0,   0, 2},
                          {'O', 20, 100, 100,   0, 2},
                          {'O',150,   0,   0,   0, 2},
                          {'L',  0,   0,   0,   0, 2}};  // 変身待機
const unsigned char ptn2[19][6]={
                          {'I',  0,   0,   0,   0, 2},
                          {'S', 50, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 50, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 30, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 30, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 60, 100, 100,   0, 2},
                          {'S',110,   0,   0,   0, 2},
                          {'S', 60, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 30, 100, 100,   0, 2},
                          {'S', 30,   0,   0,   0, 2},
                          {'S', 30, 100, 100,   0, 2},
                          {'O', 50, 100, 100,   0, 2},
                          {'O',  0,   0,   0,   0, 2},
                          {'E',  0,   0,   0,   0, 2}};  // 変身
const unsigned char ptn3[17][6]={
                          {'I', 0,   0,   0,   0, 2},
                          {'O',50, 100,   0,   0, 0},
                          {'O',50, 100,   0,   0, 1},
                          {'O',50, 100,   0,   0, 0},
                          {'O',50, 100,   0,   0, 1},
                          {'O',20, 100,   0,   0, 0},
                          {'O',20, 100,   0,   0, 1},
                          {'O',20, 100,   0,   0, 0},
                          {'O',20, 100,   0,   0, 1},
                          {'O',50, 100,   0,   0, 2},
                          {'S',50,  0,   0,   0, 2},
                          {'S',50, 100,   0,   0, 2},
                          {'S', 50, 100,   0,   0, 2},
                          {'S', 200,   0,   0,   0, 2},
                          {'S', 50, 100,   0,   0, 2},
                          {'S', 200,   0,   0,   0, 2},
                          {'E',  0,   0,   0,   0, 2}};  // フルチャージ

const unsigned char ptn4[32][6]={
                          {'I',  0,  0,   0,   0, 2},
                          {'S',100,100,  63,   6, 2},//オレンジ
                          {'S',100,  0,   0,   0, 2},                          
                          {'S',100,100, 100, 100, 2},//白
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 38, 100,  50, 2},//黄緑
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 88,  88,  88, 2},//薄いグレー
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 50,  50,  50, 2},//グレー
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 25,  25,  25, 2},//濃いグレー
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,100,   0,   0, 2},//赤
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,100,  38,  82, 2},//桃
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 63,  13, 100, 2},//紫
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 31,  82, 100, 2},//水色
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,100,  88,  13, 2},//黄色
                          {'S',100,  0,   0,   0, 2},
                          {'S',100, 63,  50,  38, 2},//茶色
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,100, 82,  63, 2},//薄紅色
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,  0,   0, 100, 2},//青
                          {'S',100,  0,   0,   0, 2},
                          {'S',100,  0, 100,   0, 2},//緑
                          {'S',100,  0,   0,   0, 2},
                          {'E', 0,   0,   0,   0, 2}};  // デモ　でもバグっている
};

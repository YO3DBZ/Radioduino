#include <Arduino.h>
#include "Wire.h"
#include "si5351.h"
#include <LiquidCrystal_I2C.h>
#include <Int64String.h>
#include "disp_draw.h"
#include "meniu_logic.h"
#include "utility.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )
Si5351 si5351;
// encoder
int pinA = 6;
int pinB = 7;
int pinC = 8;
int pinALast;
int counter = 0; //Min of 0 and Max of 19 due to one full encoder turn
unsigned long currentTime;
unsigned long loopTime;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
// rf
int mode = 1; //1 usb 2 lsb 3cw 
unsigned long long freq = 1400000000; //14Mhz
unsigned long long step_size = 10000; //100 hz
bool if1_enable = false;
unsigned long long freq_if1 = 1000000000; //10 Mhz
long long if_shift = 0 ;// for lsb so that i dont have to build 2 filters 
long long lsb_shift =240000;
long long cw_shift = 50000;
long long usb_shift = 0;
int BFO_state = 0; // 0 for off 1 for on and 2 to be eqal to if1(no shift)
unsigned long long BFO_freq = 700000000;

bool vfo = true ;
bool modifying = false;
int meniu = 0;
int r = 0;
int meniu_max = 10;
// 0 for 160m 1 for 80m 2 for 60m 3 for 40m 4 for 30m 5 for 20m 6 for 17m 7 for 15m 8 for 12m 9 for 10m 10 for 6m 11 for 4m 12 for 2m
int band = 5; 
String freq_str;


void setup() {
  pinMode (pinC,INPUT_PULLUP);
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
  pinALast = digitalRead(pinA);
  Serial.begin (9600);
  currentTime = millis();
  loopTime = currentTime;
  Serial.println();
  bool i2c_found;
  i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    while(true){
      Serial.println("Device not found on I2C bus!");
    }
  }
  si5351.set_freq(freq, SI5351_CLK0);
  // initialize the lcd
  lcd.init();
  // Turn on the Backlight
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("test");
  //ch_freq();
  vfo_update();
}


void loop() {
  r = rotate();
  // enter in meniu 
  if (digitalRead(8)== LOW && vfo == true){
    while(digitalRead(8)== LOW){
    }
    vfo = false;
    meniu_update();
  }
  //meniu render 
  if (vfo == false && r != 0 && modifying == false){
    if (r == 1){
      meniu++;
      if(meniu > meniu_max){
        meniu = 0;
      }
    }
    if (r == 2){
      meniu--;
      if (meniu < 0){
        meniu = meniu_max;
      }
    }
    meniu_update();
  }
  // ch freq by step
  if (vfo == true && r != 0){
    if (r == 1){
      ch_freq(true);
    }
    if (r == 2){
      ch_freq(false);
    }
  }
  //meniu logic
  if(vfo == false){
    meniu_logic();
  }
  //setting freq limits
  if (freq <= 40000000ULL){
    set_freq(500000ULL);
  }
  if (freq >= 14700000000ULL){
    set_freq(146000000ULL);
  }
}


















































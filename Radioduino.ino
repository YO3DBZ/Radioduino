#include <Arduino.h>
#include "Wire.h"
#include "si5351.h"
#include <LiquidCrystal_I2C.h>
#include <Int64String.h>

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

// ai generated to change
// broken when freq <<10 MHz
String proc_string(String input) {
  // 1. Cut off the last 2 digits
  if (input.length() < 2) return ""; // Safety check
  String text = input.substring(0, input.length() - 2);
  
  // Ensure we have enough length left for the inserts (at least 6 characters)
  if (text.length() < 6) return text; 
  while (text.length() < 7) {
    text = "0" + text;
  }
  int len = text.length();
  
  // 2. Extract sections based on positions from the back
  String part1 = text.substring(0, len - 6);       // Everything before the comma
  String part2 = text.substring(len - 6, len - 3); // Between comma and decimal point
  String part3 = text.substring(len - 3);          // Everything after the decimal point
  
  // 3. Rebuild with the symbols inserted
  return part1 + "." + part2 +"," + part3;
}
void set_freq(unsigned long long f){
  freq = f*100ULL;
  unsigned long long CLK0;
  unsigned long long tempIF = freq_if1 + if_shift;
  if (if1_enable == false){
    CLK0 = freq;
  }
  if(if1_enable == true){
    if(tempIF  <= freq){
      CLK0 = freq - tempIF ;
    }else{
      CLK0 = tempIF  - freq;
    }
  }
  si5351.set_freq(CLK0, SI5351_CLK0);
  //vfo_update();
}
void ch_freq(bool c){
  unsigned long long CLK0;
  unsigned long long tempIF = freq_if1 + if_shift;
  if (c== false){
    freq = freq - step_size;
  }
  if (c == true){
    freq = freq + step_size;
  }

  if (if1_enable == false){
    CLK0 = freq;
  }
  if(if1_enable == true){
    if(tempIF  <= freq){
      CLK0 = freq - tempIF ;
    }else{
      CLK0 = tempIF  - freq;
    }
  }
  si5351.set_freq(CLK0, SI5351_CLK0);
  vfo_update();
  
}

void vfo_update(){
  lcd.clear();
  freq_str = int64String(freq, 10); 
  freq_str = proc_string(freq_str);
  lcd.setCursor(0, 0);
  lcd.print(freq_str);
  lcd.setCursor(13, 0);
  lcd.print("kHz");
  lcd.setCursor(13, 1);
  if(mode ==1){
    lcd.print("USB");
  }
  if(mode ==2){
    lcd.print("LSB");
  }
  if(mode ==3){
    lcd.print("CW");
  }
}
void bfo_update(){
  if (BFO_state != 0){
    si5351.set_freq(BFO_freq, SI5351_CLK2);
  }
}
void meniu_update(){
  lcd.clear();
  String text;
  switch(meniu){
    //exit
    case 0:
      lcd.setCursor(0,0);
      lcd.print("Exit");
      break;
    //step
    case 1:
      text = int64String(step_size);
      lcd.setCursor(0, 0);
      lcd.print("Step size");
      lcd.setCursor(14, 1);
      lcd.print("Hz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + (text.substring(0, text.length() - 2)));
      }else{
        lcd.print(text.substring(0, text.length() - 2));
      }
      break;
    // mode 
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Mode");
      lcd.setCursor(0, 1);
      if(modifying == true){
        if(mode ==1){
          lcd.print(">USB");
        }
        if(mode ==2){
          lcd.print(">LSB");
        }
        if(mode ==3){
          lcd.print(">CW");
        }
      }
      else
      {
        if(mode ==1){
          lcd.print("USB");
        }
        if(mode ==2){
          lcd.print("LSB");
        }
        if(mode ==3){
          lcd.print("CW");
        }
      }
      break;
    //if1 enable
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("IF1 enable");
      lcd.setCursor(0,1);
      if(modifying == true){
        if(if1_enable == true){
          lcd.print(">ON");
        }else{
          lcd.print(">OFF");
        }
      }else{
        if(if1_enable == true){
          lcd.print("ON");
        }else{
          lcd.print("OFF");
        }
      }
      break;
    //if freq set 
    case 4:
      text = int64String(freq_if1);
      text = proc_string(text);
      lcd.setCursor(0, 0);
      lcd.print("IF 1 freq");
      lcd.setCursor(13, 1);
      lcd.print("KHz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + text);
      }else{
        lcd.print(text);
      }
      break;
    // USB shit 
    case 5:
      text = int64String(usb_shift);
      lcd.setCursor(0, 0);
      lcd.print("USB IF shift");
      lcd.setCursor(14, 1);
      lcd.print("Hz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + (text.substring(0, text.length() - 2)));
      }else{
        lcd.print(text.substring(0, text.length() - 2));
      }
      break;
    // LSB shift
    case 6:
      text = int64String(lsb_shift);
      lcd.setCursor(0, 0);
      lcd.print("LSB IF shift");
      lcd.setCursor(14, 1);
      lcd.print("Hz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + (text.substring(0, text.length() - 2)));
      }else{
        lcd.print(text.substring(0, text.length() - 2));
      }
      break;
    //cw shift
    case 7:
      text = int64String(cw_shift);
      lcd.setCursor(0, 0);
      lcd.print("CW IF shift");
      lcd.setCursor(14, 1);
      lcd.print("Hz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + (text.substring(0, text.length() - 2)));
      }else{
        lcd.print(text.substring(0, text.length() - 2));
      }
      break;
    //bfo on off
    case 8:
      lcd.setCursor(0, 0);
      lcd.print("BFO");
      lcd.setCursor(0, 1);
      if(modifying == true){
        if(BFO_state ==0){
          lcd.print(">OFF");
        }
        if(BFO_state ==1){
          lcd.print(">ON");
        }
        if(BFO_state ==2){
          lcd.print(">Eqal to if1");
        }
      }
      else
      {
        if(BFO_state==0){
          lcd.print("OFF");
        }
        if(BFO_state ==1){
          lcd.print("ON");
        }
        if(BFO_state ==2){
          lcd.print("Eqal to if1");
        }
      }
      break;
    // bfo freq set
    case 9:
      text = int64String(BFO_freq);
      text = proc_string(text);
      lcd.setCursor(0, 0);
      lcd.print("BFO freq");
      lcd.setCursor(13, 1);
      lcd.print("KHz");
      lcd.setCursor(0, 1);
      if (modifying == true){
        lcd.print(">" + text);
      }else if(modifying == true && BFO_state == 2){
        lcd.print("eqal to if");
      }
      else{
        lcd.print(text);
      }
      break;
    case 10:{
      lcd.setCursor(0, 0);
      lcd.print("Band");
      lcd.setCursor(0, 1);
      if(modifying == true){
        switch (band)
        {
          case 0:
          {
            lcd.print(">160m");
            break;
          }
          case 1:
          {
            lcd.print(">80m");
            break;
          }
          case 2:
          {
            lcd.print(">60m");
            break;
          }
          case 3:
          {
            lcd.print(">40m");
            break;
          }
          case 4:
          {
            lcd.print(">30m");
            break;
          }
          case 5:
          {
            lcd.print(">20m");
            break;
          }
          case 6:
          {
            lcd.print(">17m");
            break;
          }
          case 7:
          {
            lcd.print(">15m");
            break;
          }
          case 8:
          {
            lcd.print(">12m");
            break;
          }
          case 9:
          {
            lcd.print(">10m");
            break;
          }
          case 10:
          {
            lcd.print(">6m");
            break;
          }
          case 11:
          {
            lcd.print(">4m");
            break;
          }
          case 12:
          {
            lcd.print(">2m");
            break;
          }
        }
      }
      else
      {
        switch (band)
        {
          case 0:
          {
            lcd.print("160m");
            break;
          }
          case 1:
          {
            lcd.print("80m");
            break;
          }
          case 2:
          {
            lcd.print("60m");
            break;
          }
          case 3:
          {
            lcd.print("40m");
            break;
          }
          case 4:
          {
            lcd.print("30m");
            break;
          }
          case 5:
          {
            lcd.print("20m");
            break;
          }
          case 6:
          {
            lcd.print("17m");
            break;
          }
          case 7:
          {
            lcd.print("15m");
            break;
          }
          case 8:
          {
            lcd.print("12m");
            break;
          }
          case 9:
          {
            lcd.print("10m");
            break;
          }
          case 10:
          {
            lcd.print("6m");
            break;
          }
          case 11:
          {
            lcd.print("4m");
            break;
          }
          case 12:
          {
            lcd.print("2m");
            break;
          }
        }
      }
      break;
    }
  }
}

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

// 0 for no 1 for clockwise 2 for counterclockwise
// ai generated to change
int rotate(){
  // get the current elapsed time
  currentTime = millis();
  int rot = 0;
  if(currentTime >= (loopTime + 5)){
    encoder_A = digitalRead(pinA); // Read encoder pins
    encoder_B = digitalRead(pinB);
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low
      if(encoder_B) {
        // B is high so counter-clockwise
        counter --;
        if(counter < 0){
          counter = 20;
          rot = 2;
        }
        else
        {
        rot = 2;
        }
      }
      else {
        // B is low so clockwise
        counter ++;
        if(counter > 19){
          counter = 0;
          rot = 1;
        }
        else{
        rot = 1;
        }
      }
    }
    encoder_A_prev = encoder_A; // Store value of A for next time
    loopTime = currentTime; // Updates loopTime
  }
  return rot;
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
    switch(meniu){
      // exit meniu logic 
      case 0:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
        vfo = true;
        vfo_update();
        }
        break;
      // step select
      case 1:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else{
            modifying = false;
          }
          meniu_update();
        }
        if (r !=0 && modifying == true){
          if (r==1){
            step_size = step_size * 10ULL;
            if (step_size > 10000000ULL) step_size = 100000000ULL; // 1Mhz max
          }
          if (r==2){
            step_size = step_size / 10ULL;
            if (step_size < 100ULL) step_size = 100ULL;// 1 Hz min
          }
          meniu_update();
        }
        break;
      // mode select
      case 2:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r != 0 && modifying == true){
          if(r == 1){
            mode++;
            if (mode >= 4){
              mode = 1;
            }
          }
          if (r==2){
            mode--;
            if(mode <= 0){
              mode = 3;
            }
          }
          meniu_update();
          if(mode == 1){
            if_shift = usb_shift;
          }else if(mode == 2){
            if_shift = lsb_shift;
          }else if(mode == 3){
            if_shift = cw_shift;
          }
        }
        break; 
      //if1_enable on off
      case 3:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if(modifying== true && r!=0){
          if(r == 1){
            if(if1_enable == false){
              if1_enable= true;
            }
            else
            {
              if1_enable = false;
            }
          }else{
            if(if1_enable == false){
              if1_enable= true;
            }
            else
            {
              if1_enable = false;
            }
          }
          ch_freq(true);
          ch_freq(false);
          meniu_update();
        }
        break;
      // if1 freq set
      case 4:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r !=0 && modifying == true){
          if (r==1){
            freq_if1 = freq_if1 + step_size;
          }
          if (r==2){
            freq_if1 = freq_if1 - step_size;
            if (freq_if1 < 100ULL) freq_if1 = 10000ULL;// 100 Hz min
          }
          ch_freq(true);
          ch_freq(false);
          meniu_update();
        }
        break;
      //usb shift
      case 5:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r !=0 && modifying == true){
          if (r==1){
            usb_shift = usb_shift + step_size;
          }
          if (r==2){
            usb_shift = usb_shift - step_size;
            //if (freq_if1 < 100ULL) freq_if1 = 10000ULL;// 100 Hz min
          }
          ch_freq(true);
          ch_freq(false);
          meniu_update();
        }
        break;
      //lsb shift
      case 6:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r !=0 && modifying == true){
          if (r==1){
            lsb_shift = lsb_shift + step_size;
          }
          if (r==2){
            lsb_shift = lsb_shift - step_size;
            //if (freq_if1 < 100ULL) freq_if1 = 10000ULL;// 100 Hz min
          }
          ch_freq(true);
          ch_freq(false);
          meniu_update();
        }
        break;
      //cw shift
      case 7:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);
          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r !=0 && modifying == true){
          if (r==1){
            cw_shift = cw_shift + step_size;
          }
          if (r==2){
            cw_shift = cw_shift - step_size;
            //if (freq_if1 < 100ULL) freq_if1 = 10000ULL;// 100 Hz min
          }
          ch_freq(true);
          ch_freq(false);
          meniu_update();
        }
        break;
      //bfo on off
      case 8:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);
          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if (r != 0 && modifying == true){
          if(r == 1){
            BFO_state++;
            if (BFO_state >= 3){
              BFO_state = 0;
            }
          }
          if (r==2){
            BFO_state--;
            if(BFO_state <= -1){
              BFO_state = 2;
            }
          }
          if(BFO_state >= 1){
            si5351.output_enable(SI5351_CLK2, 1);
          }else{
            si5351.output_enable(SI5351_CLK2, 0);
          }
          if(BFO_state == 2){
            BFO_freq = freq_if1;
          }
          bfo_update();
          meniu_update();
        }
        break;
      // bfo freq set
      case 9:
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if(BFO_state !=2 ){
          if (r !=0 && modifying == true){
            if (r==1){
              BFO_freq= BFO_freq + step_size;
              if (BFO_freq > 10000000000ULL) BFO_freq = 10000000000ULL; // 100Mhz max
            }
            if (r==2){
              BFO_freq = BFO_freq - step_size;
              if (BFO_freq < 5000000ULL) BFO_freq = 5000000ULL; // 50Khz max
            }
            bfo_update();
            meniu_update();
          }
        }
        break;
      case 10:
      {
        if (digitalRead(8)== LOW){
          while(digitalRead(8)== LOW){
          }
          delay(50);

          if(modifying == false){
            modifying = true;
          }
          else
          {
            modifying = false;
          }
          meniu_update();
        }
        if(r!=0 && modifying == true){
          if (r == 1){
            band++;
            if (band >=13){
              band = 0;
            }
          }
          if (r == 2){
            band--;
            if (band < 0 ){
              band = 12;
            }
          }
          switch (band)
          {
            case 0:
            {
              set_freq(1800000);
              break;
            }
            case 1:
            {
              set_freq(3500000);
              break;
            }
            case 2:
            {
              set_freq(5351500);
              break;
            }
            case 3:
            {
              set_freq(7000000);
              break;
            }
            case 4:
            {
              set_freq(10100000);
              break;
            }
            case 5:
            {
              set_freq(14000000);
              break;
            }
            case 6:
            {
              set_freq(18068000);
              break;
            }
            case 7:
            {
              set_freq(21000000);
              break;
            }
            case 8:
            {
              set_freq(24890000);
              break;
            }
            case 9:
            {
              set_freq(28000000);
              break;
            }
            case 10:
            {
              set_freq(50000000);
              break;
            }
            case 11:
            {
              set_freq(70000000);
              break;
            }
            case 12:
            {
              set_freq(144000000);
              break;
            }
          }
          meniu_update();
        }
        break;
      }
    }
  }
  //setting freq limits
  if (freq <= 40000000ULL){
    set_freq(500000ULL);
  }
  if (freq >= 14700000000ULL){
    set_freq(146000000ULL);
  }
}


















































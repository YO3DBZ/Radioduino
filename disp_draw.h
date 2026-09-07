#include "utility.h"

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
      text = int64String((uint64_t)step_size);
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
      text = int64String((uint64_t)freq_if1);
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
      text = int64String((uint64_t)usb_shift);
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
      text = int64String((uint64_t)lsb_shift);
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
      text = int64String((uint64_t)cw_shift);
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
      text = int64String((uint64_t)BFO_freq);
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
    //band
    case 10:
    {
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
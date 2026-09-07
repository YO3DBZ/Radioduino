#include "disp_draw.h"
#include "utility.h"

void meniu_logic(){
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
      //band 
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
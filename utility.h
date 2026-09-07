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
  freq_str = int64String((uint64_t)freq); 
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


# Radioduino
Some arduino code for a superheterodyne receiver based on the SI5351 as a clock source (LO and BFO).

Originally designed as a cheap LARCSet mod to replace the LC oscillator, it derailed into a more general-purpose tool (for example as a signal generator). It was designed to be built with the minimum necessary to do such things. All you need is a rotary encoder, an I2C LCD, and the SI5351 module.

### How to connect the modules:
1. Connect the LCD to the I2C bus
2. Connect the SI5351 to the I2C bus
3. Connect rotary encoder's Clock, Data, and Switch to Arduino's pins 6, 7, and 8

To build the rest of the receiver, all you need is 2 mixers (one for the first IF and the other for the product detector), a crystal filter (for SSB), some amplifiers, etc.

### Things to do:
- [x] Add a way to change the bands
- [ ] Save the settings to EEPROM
- [ ] Add a way to change band pass filters / pull some pins high depending on the modulation
- [ ] Add CAT control
- [ ] Maybe TX?
- [ ] S-meter support
- [ ] Write some documentation so that this project could be usable by someone else

---
*Special thanks to Mihai YO3CPR and Florin YO3FLX.*
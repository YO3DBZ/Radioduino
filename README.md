# Radioduino
Some ardruino code for a superheterodyne reciver based on the SI5351 as a clock sorce (LO and BFO)

Originaly deigned as a cheap LARCSet mod to replace the LC oscilator derailed into a more general porpuse tool (for example as an signal generator). It was deigned to be built with the minimum necesary to do such things. All you need is a rotary encoder, an i2c lcd and the SI5351 module.

How to connect the modules:
1. connect the lcd to the i2c bus
2. connect the SI5351 to the i2c bus
3. connect rotary encoders Clock, Data and swich to ardruino's 6, 7, 8, pins   

To build the rest of the reciver all you need is 2 mixers (one for the first IF and the other for the product detector) a crystal filter (for ssb) some amplifiers etc..

Things to do:

1. add a way to change the bands
2. sane the settings to eeprom 
3. add a way to change band pass filters / pull some pins high when depending on the modulation
4. add CAT controll
5. maby tx?
6. s meter suport
7. write some documentation so that this projec could be useble by someone else



Special tanks to Mihai YO3CPR and Florin YO3FLX.

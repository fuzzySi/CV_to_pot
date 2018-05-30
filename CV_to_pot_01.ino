/*

  code for an arduino based modular synth module, reading CV inputs and translating the voltage to a digital pot value.
  this gives you easy control of parameters not traditionally pot controlled, or CV control of something that is normally a pot.
  
  scalable, so can be used with multiple inputs and digital pots, or DACs. change INPUTS to numbers of ADCs, and add pins to INPUT_PIN[] array
  can add extra digital pots - add extra chip select pins to OUT_CS[] array, change between single or dual channel pots with CHANNELS_PER_CHIP, 1 or 2

  hardware: 
  CV ins go into negative input of rail to rail op amp (eg MCP602), to clamp incoming voltage safely between 0 & 5v
  this is read by ADC (ATtiny 85, 10 bit resolution)
  value is translated to digital pot (MCP42xx or MCP41xx) via SPI (4x5x & 4x6x use 8 bit value, 4x3x & 4x4x are 7 bit)

  progeramming ATtiny
  ATtiny pin 2 to SCK, pin 1 to MISO, pin 0 to MOSI, reset Pin to RESET
  physical pin 7 SCK, pin 6 MISO, pin 5 MOSI, pin 1 RESET, pin 4 gnd, pin 8 V+


  Sebastian, little-scale, is usually there first:
  http://little-scale.blogspot.com/2013/05/how-to-interface-with-mcp4241-dual.html

  cc-by-sa, fuzzySi  @FuzzySynths

*/

#include <SPI.h>

//editable
const int INPUT_HYSTERESIS = 2; // by how many values on ADC incoming signal has to change before digital pot updates
//                                       increase this if CV chatter (minor fluctuations picked up by ADC) is causing a problem
// invert these if needed
const int MIN_IN_VALUE = 0; 
const int MAX_IN_VALUE = 1024;  // depends on ADC
const int MIN_OUT_VALUE = 0;
const int MAX_OUT_VALUE = 1024; // depends on digital pot

const int INPUTS = 2; // number of inputs
const int CHANNELS_PER_CHIP = 2; // using duals


// for DAC outs
const int NO_GAIN = 0x1; // 1 is no gain
const int GAIN = 0x0; // 0 is x2 gain
const int SHUTDOWN = 1; // if 0, turns digital pot off

// working vars
int outChip, outChannel;
int inVal[INPUTS], oldInVal[INPUTS], outValue[INPUTS];


// pins
// pin 2 is CLK, pin 0 is MOSI (SPI data out)
const int OUT_CS[] = {1}; // if you add extra chips here, ensure added in setup too
const int INPUT_PIN[] = {3, 4};


void setup() {
  for (int i = 0; i < INPUTS; i ++) {
      pinMode(INPUT_PIN[i], INPUT);
  }
  pinMode(OUT_CS[0], OUTPUT); // copy for each extra chip

}

void loop() {

  // readADCs
  for (int i = 0; i < INPUTS; i ++) {
    inVal[i] = analogRead(INPUT_PIN[i]);
    if (abs(oldInVal[i] - inVal[i]) > INPUT_HYSTERESIS) { //                                 changed
      oldInVal[i] = inVal[i];
      outValue[i] = map(inVal[i], MIN_IN_VALUE, MAX_IN_VALUE, MIN_OUT_VALUE, MAX_IN_VALUE);
      outChip = int(i / CHANNELS_PER_CHIP); // for dual pots, if single pots, outChip = i;
      outChannel = i % CHANNELS_PER_CHIP;
      
      // if CV in has changed, write to the output
      writeMCP42xx(outChip, outChannel, outValue[i]);
      //                                                                  setOutput(outChip, outChannel, GAIN, outValue[i]);
      //                                                                      // ^ this does DAC out to MCP4922
    }
  }
  // just keep doing this for ever
}

void writeMCP42xx(byte chip, byte address, byte value) {
  digitalWrite(OUT_CS[chip], LOW);
  SPI.transfer(address << 4);
  SPI.transfer(value & 127);
  digitalWrite(OUT_CS[chip], HIGH);
}


/*
  // this is for DAC out

  void setOutput(byte chip, byte channel, byte gain, unsigned int val) // bit 12 shutdown 1 is active, 0 is no output
  {
  byte lowByte = val & 0xff;
  byte highByte = ((val >> 8) & 0xff) | channel << 7 | gain << 5 | SHUTDOWN << 4; // 7 is bit 15, 5 bit 13. gain 0 is 2x

    digitalWrite(DIGIPOT_CS[chip], LOW);
    SPI.transfer(highByte);
    SPI.transfer(lowByte);
     digitalWrite(DIGIPOT_CS[chip], HIGH);

  }
*/

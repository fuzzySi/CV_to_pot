# CV_to_pot

  code for an arduino based modular synth module, reading CV inputs and translating the voltage to a digital pot value.
  this gives you easy control of parameters not traditionally pot controlled, or CV control of something that is normally a pot.
  
  scalable, so can be used with multiple inputs and digital pots, or DACs. change INPUTS to numbers of ADCs, and add pins to INPUT_PIN[] array
  
  can add extra digital pots - add extra chip select pins to OUT_CS[] array, change between single or dual channel pots with CHANNELS_PER_CHIP, 1 or 2
  
  hardware: 
  
  CV ins go into negative input of rail to rail op amp (eg MCP602), to clamp incoming voltage safely between 0 & 5v
  
  this is read by ADC (ATtiny 85, 10 bit resolution)
  
  value is translated to digital pot (MCP42xx or MCP41xx) via SPI 
  (MCP4x5x & MCP4x6x use 8 bit value, MCP4x3x & MCP4x4x are 7 bit)
  
  
  programming ATtiny
  
  ATtiny pin 2 to SCK, pin 1 to MISO, pin 0 to MOSI, reset Pin to RESET
  physical pin 7 SCK, pin 6 MISO, pin 5 MOSI, pin 1 RESET, pin 4 gnd, pin 8 V+

	little-scale, is usually there first:
  http://little-scale.blogspot.com/2013/05/how-to-interface-with-mcp4241-dual.html
  cc-by-sa, fuzzySi  @FuzzySynths


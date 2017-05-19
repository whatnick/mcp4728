/* 
Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
*/

#include <Wire.h>
#include "mcp4728.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
const float pi = 3.14;

void setup()
{
  Serial.begin(9600);  // initialize serial interface for print()
  dac.begin();  // initialize i2c interface
  Wire.setClock(400000L);
  dac.vdd(3300); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout

/* If LDAC pin is not grounded, need to be pull down for normal operation.

  int LDACpin = 8;
  pinMode(LDACpin, OUTPUT); 
  digitalWrite(LDACpin, LOW);
*/
  printStatus(); // Print all internal value and setting of input register and EEPROM. 
}
const int period = 75;
void loop()
{
  for(int i=0;i<=period;i++)
  {
    int val = 2048+2048*cos(i*3.1415*2/period);
    dac.analogWrite(val,val,val,val); // write to input register of DAC four channel (channel 0-3) together. Value 0-4095
  }
}

void printStatus()
{
  Serial.println("NAME     Vref  Gain  PowerDown  Value");
  for (int channel=0; channel <= 3; channel++)
  { 
    Serial.print("DAC");
    Serial.print(channel,DEC);
    Serial.print("   ");
    Serial.print("    "); 
    Serial.print(dac.getVref(channel),BIN);
    Serial.print("     ");
    Serial.print(dac.getGain(channel),BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDown(channel),BIN);
    Serial.print("       ");
    Serial.println(dac.getValue(channel),DEC);

    Serial.print("EEPROM");
    Serial.print(channel,DEC);
    Serial.print("    "); 
    Serial.print(dac.getVrefEp(channel),BIN);
    Serial.print("     ");
    Serial.print(dac.getGainEp(channel),BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDownEp(channel),BIN);
    Serial.print("       ");
    Serial.println(dac.getValueEp(channel),DEC);
  }
  Serial.println(" ");
}

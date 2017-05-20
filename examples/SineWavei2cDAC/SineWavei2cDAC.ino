/* 
Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
*/

#include <Wire.h>
#include "mcp4728.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
const float pi = 3.141529;

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
const int sine[75] = {0x800,0x8ad,0x95a,0xa03,0xaaa,0xb4b,0xbe6,0xc7a,
0xd06,0xd88,0xe01,0xe6e,0xecf,0xf24,0xf6b,0xfa5,
0xfd1,0xfee,0xffd,0xffd,0xfee,0xfd1,0xfa5,0xf6b,
0xf24,0xecf,0xe6e,0xe01,0xd88,0xd06,0xc7a,0xbe6,
0xb4b,0xaaa,0xa03,0x95a,0x8ad,0x800,0x752,0x6a5,
0x5fc,0x555,0x4b4,0x419,0x385,0x2f9,0x277,0x1fe,
0x191,0x130,0xdb,0x94,0x5a,0x2e,0x11,0x2,
0x2,0x11,0x2e,0x5a,0x94,0xdb,0x130,0x191,
0x1fe,0x277,0x2f9,0x385,0x419,0x4b4,0x555,0x5fc,
0x6a5,0x752,0x800};
void loop()
{
  for(int i=0;i<period;i++)
  {
    int val = sine[i];
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

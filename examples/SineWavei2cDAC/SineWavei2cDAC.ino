/*
  Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
  For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
  Better driver now available via Adafruit, and this code may be ported to it in the near future.
  https://learn.adafruit.com/adafruit-mcp4728-i2c-quad-dac?view=all
*/
#include "mcp4728.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
const float pi = 3.141529;
bool first_run = true;

const int period = 165;
const int off1 = period / 3;
const int off2 = 2 * period / 3;

// Sine waveform lookup
const int sine[period] = {0x800, 0x84e, 0x89d, 0x8eb, 0x939, 0x986, 0x9d3, 0xa1f,
                          0xa6a, 0xab4, 0xafe, 0xb46, 0xb8d, 0xbd2, 0xc17, 0xc59,
                          0xc9a, 0xcda, 0xd17, 0xd53, 0xd8c, 0xdc4, 0xdf9, 0xe2c,
                          0xe5d, 0xe8b, 0xeb7, 0xee0, 0xf07, 0xf2b, 0xf4d, 0xf6c,
                          0xf87, 0xfa1, 0xfb7, 0xfca, 0xfdb, 0xfe8, 0xff2, 0xffa,
                          0xffe, 0x1000, 0xffe, 0xffa, 0xff2, 0xfe8, 0xfdb, 0xfca,
                          0xfb7, 0xfa1, 0xf87, 0xf6c, 0xf4d, 0xf2b, 0xf07, 0xee0,
                          0xeb7, 0xe8b, 0xe5d, 0xe2c, 0xdf9, 0xdc4, 0xd8c, 0xd53,
                          0xd17, 0xcda, 0xc9a, 0xc59, 0xc17, 0xbd2, 0xb8d, 0xb46,
                          0xafe, 0xab4, 0xa6a, 0xa1f, 0x9d3, 0x986, 0x939, 0x8eb,
                          0x89d, 0x84e, 0x800, 0x7b2, 0x763, 0x715, 0x6c7, 0x67a,
                          0x62d, 0x5e1, 0x596, 0x54c, 0x502, 0x4ba, 0x473, 0x42e,
                          0x3e9, 0x3a7, 0x366, 0x326, 0x2e9, 0x2ad, 0x274, 0x23c,
                          0x207, 0x1d4, 0x1a3, 0x175, 0x149, 0x120, 0xf9, 0xd5,
                          0xb3, 0x94, 0x79, 0x5f, 0x49, 0x36, 0x25, 0x18,
                          0xe, 0x6, 0x2, 0x0, 0x2, 0x6, 0xe, 0x18,
                          0x25, 0x36, 0x49, 0x5f, 0x79, 0x94, 0xb3, 0xd5,
                          0xf9, 0x120, 0x149, 0x175, 0x1a3, 0x1d4, 0x207, 0x23c,
                          0x274, 0x2ad, 0x2e9, 0x326, 0x366, 0x3a7, 0x3e9, 0x42e,
                          0x473, 0x4ba, 0x502, 0x54c, 0x596, 0x5e1, 0x62d, 0x67a,
                          0x6c7, 0x715, 0x763, 0x7b2, 0x800};

int lut_index = 0;

void setup()
{
  Serial.begin(9600); // initialize serial interface for print()
  Serial.println("Initializing DAC");
  dac.begin(); // initialize i2c interface
  Wire.setClock(3200000L);
  dac.vdd(3300); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout

  /* If LDAC pin is not grounded, need to be pull down for normal operation.

    int LDACpin = 8;
    pinMode(LDACpin, OUTPUT);
    digitalWrite(LDACpin, LOW);
  */
  delay(1000);
  printStatus();
}

void loop()
{
  int va = sine[lut_index];
  int vb = sine[(lut_index + off1) % period];
  int vc = sine[(lut_index + off2) % period];
  dac.analogWrite(va, vb, vc, va); // write to input register of DAC four channel (channel 0-3) together. Value 0-4095
  lut_index++;
  if (lut_index >= period)
  {
    lut_index = 0;
  }
}

void printStatus()
{
  Serial.println("NAME     Vref  Gain  PowerDown  Value");
  for (int channel = 0; channel <= 3; channel++)
  {
    Serial.print("DAC");
    Serial.print(channel, DEC);
    Serial.print("   ");
    Serial.print("    ");
    Serial.print(dac.getVref(channel), BIN);
    Serial.print("     ");
    Serial.print(dac.getGain(channel), BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDown(channel), BIN);
    Serial.print("       ");
    Serial.println(dac.getValue(channel), DEC);

    Serial.print("EEPROM");
    Serial.print(channel, DEC);
    Serial.print("    ");
    Serial.print(dac.getVrefEp(channel), BIN);
    Serial.print("     ");
    Serial.print(dac.getGainEp(channel), BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDownEp(channel), BIN);
    Serial.print("       ");
    Serial.println(dac.getValueEp(channel), DEC);
  }
  Serial.println(" ");
}
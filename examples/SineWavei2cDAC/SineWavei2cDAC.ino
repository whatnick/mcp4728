/*
  Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
  For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
  Better driver now available via Adafruit, and this code may be ported to it in the near future.
  https://learn.adafruit.com/adafruit-mcp4728-i2c-quad-dac?view=all
*/
#include "mcp4728.h"

#define DEG2RAD 0.01745               // Convenience conversion factor
#define STEP_SIZE 2.25                // Use this variable to fine tune frequency
#define LUT_SIZE int(360 / STEP_SIZE) // Useful for LUT dimensioning

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

// Sine waveform lookup
int sine_a[LUT_SIZE];
int sine_b[LUT_SIZE];
int sine_c[LUT_SIZE];

int lut_index = 0;

void setup()
{
  Serial.begin(115200); // initialize serial interface for print()
  Serial.println("Initializing DAC");
  dac.begin(); // initialize i2c interface
  Wire.setClock(3200000L);
  dac.vdd(3300);  // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  prep_loopkup(); // Prepare 3 LUT tables for the phases
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
  int va_sin = sine_a[lut_index];
  int vb_sin = sine_b[lut_index];
  int vc_sin = sine_c[lut_index];

  dac.analogWrite(va_sin, vb_sin, vc_sin, va_sin); // write to input register of DAC four channel (channel 0-3) together. Value 0-4095

  lut_index++;
  if (lut_index >= LUT_SIZE)
  {
    lut_index = 0;
  }

  print_dac(va_sin, vb_sin, vc_sin);
}

void prep_loopkup()
{
  for (int i = 0; i < LUT_SIZE; i++)
  {
    int va_sin = sin(i * STEP_SIZE * DEG2RAD) * 1200 + 2048;
    int vb_sin = sin((i * STEP_SIZE + 120) * DEG2RAD) * 1200 + 2048;
    int vc_sin = sin((i * STEP_SIZE + 240) * DEG2RAD) * 1200 + 2048;
    sine_a[i] = va_sin;
    sine_b[i] = vb_sin;
    sine_c[i] = vc_sin;
  }
}

void print_dac(int va, int vb, int vc)
{
  Serial.print(va);
  Serial.print(",");
  Serial.print(vb);
  Serial.print(",");
  Serial.println(vc);
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
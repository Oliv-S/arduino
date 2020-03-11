#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

#define x1Bit 0.0078125

//Voltmeter
int voltmeterPin = 1;
const int voltmeterBufferSize = 100;
int voltmeterBufferCurrentPosition = 0;
int16_t voltmeterBuffer[voltmeterBufferSize];

int voltmeterNullValue = 0;

void setup(void) 
{
  Serial.begin(115200);
  ads.setGain(GAIN_SIXTEEN);
  ads.begin();
  
  //Voltmeter
  int16_t firstValue = readRawVoltmeterValueFromAdc();  
  Serial.println(firstValue);
  for (int i = 0; i < voltmeterBufferSize; i = i + 1) {
    voltmeterBuffer[i] = firstValue;
  };
  

  Serial.println("Hello!");
  
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV


  calibrateVoltmeter();
}

void loop(void) 
{
  int16_t adc0, adc1, adc2, adc3;

  //double volt0 = ( (double)adc0 * (double)x1Bit ) / (double)1000.0;
  
  Serial.print(getRawVoltmeterValue());Serial.print(" - ");
  Serial.print(readRawVoltmeterValueFromAdc());Serial.print("; ");
  Serial.print(getNormalizedVoltmeterValue());Serial.println("; ");
  delay(200);
}

void calibrateVoltmeter()
{
  int calibratingBufferSize = 50;
  int calibratingBufferPosition = 0;
  int16_t calibratingBuffer[calibratingBufferSize];
  bool calibrated = false;
  
  int counter = 0;
  while (!calibrated && counter<1000)
  {
    int16_t currentValue = getRawVoltmeterValue();
    Serial.println(currentValue);
    calibratingBuffer[calibratingBufferPosition] = currentValue;
    calibratingBufferPosition+=1;
    if (calibratingBufferPosition==calibratingBufferSize)
    {
      calibratingBufferPosition=0;
    }
    calibrated = true;
    for (int i=0; i<calibratingBufferSize; i++)
    {
      if (calibratingBuffer[i]!=currentValue)
      {
        calibrated=false;
      }
    }
    counter++;
  }
  if (calibrated)
  {
    Serial.println("Calibrated!");
    Serial.print("Null value = ");Serial.println(calibratingBuffer[0]);
  }
  voltmeterNullValue = calibratingBuffer[0];
}
void saveVoltmeterValue( int16_t val)
{
  voltmeterBuffer[voltmeterBufferCurrentPosition] = val;
  voltmeterBufferCurrentPosition++;
  if (voltmeterBufferCurrentPosition>=voltmeterBufferSize)
  {
    voltmeterBufferCurrentPosition=0;
  }
}

int16_t readRawVoltmeterValueFromAdc()
{
  return (ads.readADC_SingleEnded(0)+ads.readADC_SingleEnded(1)+ads.readADC_SingleEnded(2)+ads.readADC_SingleEnded(3))/4;
}

int16_t getRawVoltmeterValue()
{
  
  saveVoltmeterValue(readRawVoltmeterValueFromAdc());
  long sum = 0;
  for (int i = 0; i < voltmeterBufferSize; i = i + 1) {
    sum += voltmeterBuffer[i];
  }
  return (1.0 * sum) / voltmeterBufferSize;
}

int16_t getNormalizedVoltmeterValue()
{
  float currValue = getRawVoltmeterValue()-voltmeterNullValue;
/*  if (currValue<0)
  {
    currValue = 0.0;
  }
  */
  return currValue;
}

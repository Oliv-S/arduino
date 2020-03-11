// MPU-6050 Short Example Sketch
//www.elegoo.com
//2016.12.9


#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Voltmeter
int voltmeterPin = 1;
const int voltmeterBufferSize = 20;
int voltmeterBufferCurrentPosition = 0;
int voltmeterBuffer[voltmeterBufferSize];

int voltmeterNullValue = 0;

void setup(){
  //Voltmeter  
  analogReference(INTERNAL);
  for (int i = 0; i < voltmeterBufferSize; i = i + 1) {
    voltmeterBuffer[i] = 0;
  };
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Laser power");
  lcd.setCursor(0, 1);
  lcd.print("meter v 1.0");
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Self calibration...");
  delay(100);



  calibrateVoltmeter();

}

void loop(){  
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 1);
  lcd.print("P = " + String(getVoltmeterValue()) + "         ");

  delay(100);
}

void calibrateVoltmeter()
{
  static int voltmeterPrevValue = -1;
  int counter = 0;
  while (voltmeterPrevValue != voltmeterNullValue || counter<50 )
  {
    voltmeterPrevValue = voltmeterNullValue;
    voltmeterNullValue = getRawVoltmeterValue();
    counter +=1;
    delay(20);
  }
}
void saveVoltmeterValue( int val)
{
  voltmeterBuffer[voltmeterBufferCurrentPosition] = val;
  voltmeterBufferCurrentPosition++;
  if (voltmeterBufferCurrentPosition==voltmeterBufferSize)
  {
    voltmeterBufferCurrentPosition=0;
  }
}

int getRawVoltmeterValue()
{
  saveVoltmeterValue(analogRead(voltmeterPin));
  
  long sum = 0;
  for (int i = 0; i < voltmeterBufferSize; i = i + 1) {
    sum += voltmeterBuffer[i];
  }
  return (1.0 * sum) / voltmeterBufferSize;
}

float getVoltmeterValue()
{
  float currValue = getRawVoltmeterValue()-voltmeterNullValue;
  if (currValue<0)
  {
    currValue = 0.0;
  }
  return currValue;
}

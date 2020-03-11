
#include <LiquidCrystal.h>

volatile boolean ButtonOKPressed;  // need volatile for Interrupts

const int longButtonDelay = 40;
const int shortButonDelay = 1;

//LCD
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Variables
int analogPin = 1;

//Buttons
const int buttonOK = 2;
const int buttonOKL = 12;
int buttonOKCnt = 0;

const int buttonPlus = 3;
const int buttonPlusL = 13;
int buttonPlusCnt = 0;

const int buttonMinus = 4;
const int buttonMinusL = 14;
int buttonMinusCnt = 0;

const int buttonLeft = 6;
const int buttonLeftL = 16;
int buttonLeftCnt = 0;

const int buttonRight = 5;
const int buttonRightL = 15;
int buttonRightCnt = 0;

int buttonCurrent = 0;

//Mode
const int modeTestId    = 1;
const int modeLogId     = 2;
const int modeOptionsId = 3;
const int modeSetId     = 4;
const int modeResetId   = 5;

int currentMode = modeTestId;
int prevMode = modeTestId;

void setup() {
  //Setup Buttons
  initButtons();
  initLCD();
}

void loop() {
  readButtons();
  switch (getCurrentMode())
  {
    case modeTestId:
    modeTest();
    break;

    case modeLogId:
    modeLog();
    break;

    case modeOptionsId:
    modeOptions();
    break;

    case modeSetId:
    modeSet();
    break;

    case modeResetId:
    modeReset();
    break;

    
  }
  
  delay(20);
  
  //lcd.setCursor(0, 0);
  //lcd.print("test");
}
void modeReset()
{
  showMode("Rst.");
  showStatus("");
  switch (getPressedButton())
  {
    case buttonOK:
     setCurrentMode(modeTestId);
     break;
  }
}
void modeOptions()
{
  showMode("Opt.");
  showStatus("");
  switch (getPressedButton())
  {
    case buttonOK:
     setCurrentMode(modeTestId);
     break;

  }
}
void modeSet()
{
  showMode("Set");
  showStatus("");
  switch (getPressedButton())
  {
    case buttonOK:
     setCurrentMode(getPrevMode());
     break;
    case buttonOKL:
     //Save new Target
     setCurrentMode(getPrevMode());
     break;
  }
}
void modeLog()
{
  showMode("Log");
  showStatus("");
  switch (getPressedButton())
  {
    case buttonOK:
     setCurrentMode(modeOptionsId);
     break;
    case buttonOKL:
     setCurrentMode(modeSetId);
     break;
  }
}
void modeTest()
{
  static int modeStatus = 0;
  showMode("Test");
  switch (modeStatus)
  {
    case 0:
     showStatus("Wait");
     break;
    case 1:
     showStatus("Ready");
     break;
    case 2:
     showStatus("Go");
     break;
    case 3:
     showStatus("End");
     break;
  }
  switch (getPressedButton())
  {
    case buttonOK:
     setCurrentMode(modeLogId);
     break;
    case buttonOKL:
     modeStatus += 1;
     if (modeStatus>3) modeStatus = 0;
     //setCurrentMode(modeSetId);
     break;
  }
}
int getCurrentMode()
{
  return currentMode;
}
int getPrevMode()
{
  return prevMode;
}
void setCurrentMode(int mode)
{
  prevMode = currentMode;
  currentMode = mode;
}
void  initLCD()
{
  lcd.begin(16, 2);
  lcd.clear();
}

void showFirstLine(String text)
{
   text = text + "           ";
   showMsg( text.substring(0,11), 0, 0 );  
}
void showSecondLine(String text)
{
   text = text + "           ";
   showMsg( text.substring(0,11), 0, 1 );  
}
void showMode(String text)
{
   text = text + "     ";
   showMsg( text.substring(0,5), 11, 0 );  
}
void showStatus(String text)
{
   text = text + "     ";
   showMsg( text.substring(0,5), 11, 1 );  
}
void showMsg(String text, int x, int y)
{
   lcd.setCursor(x, y);
   lcd.print( text );
  
}
void initButtons()
{
  pinMode(buttonOK, INPUT_PULLUP);
  pinMode(buttonPlus, INPUT_PULLUP);
  pinMode(buttonMinus, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
}

void readButtons()
{
  checkButton(buttonOK, buttonOKCnt, "OK   ");
  checkButton(buttonPlus, buttonPlusCnt, "Plus ");
  checkButton(buttonMinus, buttonMinusCnt, "Minus");
  checkButton(buttonLeft, buttonLeftCnt, "Left ");
  checkButton(buttonRight, buttonRightCnt, "Right");
}

int getPressedButton()
{
  int tmp = buttonCurrent;
  buttonCurrent = 0;
  return tmp;
}

void checkButton(int button, int &cnt, String msg)
{
  if (digitalRead(button) == LOW)
  {
    // check if button is pressed
    cnt = cnt + 1;
  }
  else
  {
    if (cnt > 40)
    {
      //Button long pressed
      buttonCurrent = button + 10;
    }
    else if (cnt > 1)
    {
      //Button short presssed
      buttonCurrent = button;
    }
    cnt = 0;
  }
}

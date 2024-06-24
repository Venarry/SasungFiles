#include <BleKeyboard.h>

const int ActivateTimeout = 5000;
int moveSensorSignalPort = 13;
int activeStateSignalPort = 35;
int alarmPort = 33;
uint32_t lastActivateTime;
BleKeyboard bleKeyboard;

void setup() 
{
  Serial.begin(115200);
  pinMode(moveSensorSignalPort, INPUT_PULLUP);
  pinMode(activeStateSignalPort, INPUT_PULLUP);
  pinMode(alarmPort, OUTPUT);
  bleKeyboard.begin();
}

void loop()
{
  delay(100);
  
  int activeStateSignal = digitalRead(activeStateSignalPort);

  if(activeStateSignal == LOW)
  {
    return;
  }

  int signal = analogRead(moveSensorSignalPort);

  if(signal >= 400)
  {
    if(millis() - lastActivateTime >= ActivateTimeout)
    {
      TryHandleKeyboard();
      EnableAlarm();

      /*int toneDuration = ActivateTimeout/5;

      tone(alarmPort, 350, toneDuration);
      delay(toneDuration);

      tone(alarmPort, 250, toneDuration);
      delay(toneDuration);

      tone(alarmPort, 350, toneDuration);
      delay(toneDuration);

      tone(alarmPort, 250, toneDuration);*/
    }
  }
}

void TryHandleKeyboard()
{
  if(bleKeyboard.isConnected())
  {
    bleKeyboard.press(KEY_LEFT_GUI);
    delay(100);

    int keyD = 100;
    int keyL = 108;
    bleKeyboard.press(keyL);
    delay(100);

    bleKeyboard.releaseAll();

    lastActivateTime = millis();
  }
}

void EnableAlarm()
{
  int alarmStep = 10;
  int alarmDirection = alarmStep;
  int additionalFrequency = 0;
  int maxAdditionalFrequency = 350;
  int alarmSpeed = 5;

  for(int i = 0; i < ActivateTimeout/alarmSpeed; i++)
  {
    additionalFrequency += alarmDirection;

    if(additionalFrequency >= maxAdditionalFrequency && alarmDirection == alarmStep)
    {
      alarmDirection = -alarmStep;
    }

    if(additionalFrequency <= 0 && alarmDirection == -alarmStep)
    {
      alarmDirection = alarmStep;
    }

    int baseFrequency = 10;
    tone(alarmPort, baseFrequency + additionalFrequency);
    delay(alarmSpeed);
    noTone(alarmPort);
  }
}
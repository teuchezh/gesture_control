#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include "HID-Project.h"

// Pins
#define APDS9960_INT 20 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup()
{

  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true))
  {
    Serial.println(F("Gesture sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  System.begin();
}

void loop()
{
  if (isr_flag == 1)
  {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine()
{
  isr_flag = 1;
}

void handleGesture()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      Consumer.write(MEDIA_VOL_UP);
      Serial.println("UP");
      break;
    case DIR_DOWN:
      Consumer.write(MEDIA_VOL_DOWN);
      Serial.println("DOWN");
      break;
    case DIR_LEFT:
      Consumer.write(MEDIA_NEXT);
      Serial.println("LEFT");
      break;
    case DIR_RIGHT:
      Consumer.write(MEDIA_PREVIOUS);
      Serial.println("RIGHT");
      break;
    case DIR_NEAR:
      Serial.println("NEAR");
      break;
    case DIR_FAR:
      Serial.println("FAR");
      break;
    default:
      Serial.println("NONE");
    }
  }
}

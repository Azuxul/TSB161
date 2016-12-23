/**
 * TSB161 TEMPERATURE SENSOR BOX Arduino software
 * Read sensors values for prossesing and 
 * getting temperature data in degres celus.
 * 
 * Works with a PT100 and thermistor(MJSTS-103-3950-1-600-3D)
 * This software is devloped for Intel Edison board only
 * 
 * Devloped and tested by Lancelot H.
 * Copyright Lancelot H. (Azuxul) 2016, All Rights Reserved.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Prepare 20*4 char lcd

#define ANALOG_RESOLUTION_BITS 12
#define MAX_VOLTAGE_VALUE 4095 // 1023 for 10 bits controllers and 4095 for 12bits controllers

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.home();

  lcd.setCursor(1, 0);
  lcd.print("Temperature Sensor");
  lcd.setCursor(1, 1);
  lcd.print("Box");
  lcd.setCursor(1, 3);
  lcd.print("2016 - Lancelot H.");

  delay(5000);

  analogReadResolution(ANALOG_RESOLUTION_BITS);
}

void loop() {

  // Thermistor 

  float voltage = ((float) analogRead(A0)/(float) MAX_VOLTAGE_VALUE) * 5.0;
  float ohm = voltage / ((5.0-voltage) / 10000.0);
  float temperature = 1.0/(1.110670070e-3 + 2.362335809e-4 * log(ohm) + 0.8638827148e-7 * pow(log(ohm), 3.0)) - 273.15;

  lcd.clear();
  lcd.home();
  lcd.print("Thermistor: ");
  lcd.setCursor(0, 1);
  lcd.print(ohm / 1000.0);
  lcd.print(" kOhm, ");
  lcd.print(temperature);
  lcd.print(" C");

  // PT100
  // TODO: PT100 prossesing
  
  voltage = ((float) analogRead(A1)/(float) MAX_VOLTAGE_VALUE) * 5.0;
  ohm = voltage / ((5.0-voltage) / 10000.0);
  temperature = -1;

  lcd.setCursor(0, 2);
  lcd.print("PT100: ");
  lcd.setCursor(0, 3);
  lcd.print(ohm / 1000.0);
  lcd.print(" kOhm, ");
  lcd.print(temperature);
  lcd.print(" C");
  
  delay(1000);
}


/**
 * TSB161 TEMPERATURE SENSOR BOX Arduino software
 * Read sensors values for prossesing and 
 * getting temperature data in degres celus.
 * 
 * This version save data in file, change DATA_SAVE 
 * to false for disable.
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
#define DATA_SAVE true

FILE *dataOut;
long timeData = 0;
char fileName[26];

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(2));

  // Data save init
  
  if(DATA_SAVE) {
    String fileNameString = "temperature-data-";
    fileNameString = fileNameString + random(10000);
    fileNameString = fileNameString + ".csv";

    fileNameString.toCharArray(fileName, 26);

    dataOut = fopen(fileName, "w+");
    fprintf(dataOut, "%s", "Time,Thermistor temperature (°C),Thermistor resistance (ohm),PT100 temperature (°C),PT100 resistance (ohm)\n");
    fclose(dataOut); 
  }

  // Screen init

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
  lcd.print("Thermistor:  T=");
  lcd.print(timeData);
  lcd.setCursor(0, 1);
  lcd.print(ohm / 1000.0);
  lcd.print(" kOhm, ");
  lcd.print(temperature);
  lcd.print(" C");

  float thermistorOhm = ohm;
  float thermistorTemperature = temperature;

  // PT100
  
  voltage = ((float) analogRead(A1)/(float) MAX_VOLTAGE_VALUE) * 5.0;
  ohm = voltage / ((5.0-voltage) / 10000.0);
  if(ohm > 10000) { // Remove bad ohm value for remove screen overflow
    ohm = 0;
  }
  temperature = (ohm/100 - 1)/0.003850;

  lcd.setCursor(0, 2);
  lcd.print("PT100: ");
  lcd.setCursor(0, 3);
  lcd.print(ohm);
  lcd.print(" Ohm, ");
  lcd.print(temperature);
  lcd.print(" C");
  
  delay(1000);

  // Data saving
  
  if(DATA_SAVE) {
    dataOut = fopen(fileName, "r+w");
    
    fseek(dataOut, 0, SEEK_END);
    fprintf(dataOut, "%d,%f,%f,%f,%f\n", timeData, thermistorTemperature, thermistorOhm, temperature, ohm);
    fclose(dataOut);
  }
  
  timeData++;
}


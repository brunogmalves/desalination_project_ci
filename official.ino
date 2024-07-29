#include "Arduino.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "time.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "freertos/task.h"
#include "GravityTDS.h"
#include "driver/adc.h"

int adcRange = 4096;
float vref = 5;
float voltage = 0;
float temperature = 25.6;
int tdsValue = 0.5;
float kValue = 0.3;
int oldMillis = 0;
float calibrate_num, w_conc;
int interval = 1000;
float ecValue, ecValue25 = 0;
int des_conc = 300;
#define sensorPin GPIO_NUM_36
#define outputPin GPIO_NUM_22

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = 0;
  if (millis() - oldMillis > interval) {
    oldMillis = millis();
    for (int i = 0; i < 100; i++) {
      analogValue += analogRead(sensorPin);
    }
  analogValue = analogValue/100;
  voltage = analogValue*vref/adcRange;
  ecValue = (133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage)*kValue;
  ecValue25 = ecValue / (1.0+0.02*(temperature-25.0));
  tdsValue = ecValue25 * TdsFactor;
  Serial.print(tdsValue);
  Serial.println("ppm");
  if (tdsValue > des_conc) {
      digitalWrite(outputPin, LOW);
    } else {
      digitalWrite(outputPin, HIGH);
    }
  }
}
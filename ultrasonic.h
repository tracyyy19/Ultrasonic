#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define TRIGGER_PIN PB4   // 12
#define ECHO_PIN PB0      //8

void setupPins();
void setupTimer1();
void triggerSensor();
unsigned long measureDistance_cm();
float distance_in_inch(float cm);

#endif
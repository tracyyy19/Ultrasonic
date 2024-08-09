#include "ultrasonic.h"

void setupPins(){
  DDRB |= (1 << TRIGGER_PIN); 
  DDRB &= ~(1 << ECHO_PIN);   
}

void setupTimer1() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS11) | (1 << ICES1); // Set 8 prescaler, input capture on rising edge
  TIFR1 |= (1 << ICF1); // Clear Input Capture Flag
}

void triggerSensor() {
  PORTB &= ~(1 << TRIGGER_PIN); // Set trigger low
  delayMicroseconds(2);
  PORTB |= (1 << TRIGGER_PIN);  // Set trigger high
  delayMicroseconds(10);
  PORTB &= ~(1 << TRIGGER_PIN); // Set trigger low
}


unsigned long measureDistance_cm() {
  unsigned long startTime = 0, endTime = 0;
  float pulseWidth;

  // Wait for rising edge capture
  while (!(TIFR1 & (1 << ICF1)));
  startTime = ICR1;
  TIFR1 |= (1 << ICF1); // Clear the capture flag
  TCCR1B &= ~(1 << ICES1); // Set to capture on falling edge

  // Wait for falling edge capture
  while (!(TIFR1 & (1 << ICF1)));
  endTime = ICR1;
  TIFR1 |= (1 << ICF1); // Clear the capture flag
  TCCR1B |= (1 << ICES1); // Set to capture on rising edge

  // Handle potential timer overflow
  if (endTime < startTime) {
    pulseWidth = (0xFFFF - startTime + endTime + 1);
    //Serial.println("OV");
  } else {
    pulseWidth = endTime - startTime;
  }
  
  // Calculate distance in cm from pulse width
  // pulseWidth in timer ticks, *0.5 µs per tick, *speed of sound in cm/µs, /2 for round trip
  float distance = pulseWidth * 0.5 * 0.0343 / 2;
  //Serial.print("Distance (cm): ");
  //Serial.println(distance);
  return distance;  
}

float distance_in_inch(float cm){
  float distance_inch = cm*0.3937; //convert cm to inches
  //Serial.print("Distance (in): ");
  //Serial.println(distance_inch);
  return distance_inch;
}

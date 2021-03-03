#include <Wire.h>
#include <Servo.h>
#include <Encoder.h>
#include <HCSR04.h>
Servo serv;
  
void setup() {
  serv.attach(4); //TODO: attach servo pin

 
}

void loop() {
 

  serv.write(130); // set servo to angle pos
}

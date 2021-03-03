#include <Wire.h>
#include <Servo.h>
Servo serv;
  
void setup() {
  serv.attach(4); //TODO: attach servo pin
  serv.write(90); // set servo to angle pos

}

void loop() {
 for (int i = 90; i > 0; i--){
  serv.write(i);
  delay(5);
 }
 for (int i = 0; i < 180; i++){
  serv.write(i);
  delay(5);
 }
 for (int i = 180; i > 90; i--){
  serv.write(i);
  delay(5);
 }
 delay(5000);
}

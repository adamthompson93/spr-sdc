#include <Wire.h>
#include <Servo.h>
#include <Encoder.h>
int in1, in2, in3, in4;    //motor controller input pins
int out1, out2, out3, out4;
int enabler1, enabler2;   //motor controller enable pins
int ir1, ir2, ir3, ir4; // ir1 and 2 are top left and right, ir3 and 4 are bottom left and right
Servo serv;
Encoder enc(2, 3);


/*
    Drives the car using the motor controller commands
    Inputs:
      [char] dir: motor direction commands
        {'b': brake, 'r': reverse, 'f': forward, 'c': coast}
      [int] speed: motor speed (full speed is 255)
        {0 <= speed <= 255}
*/

void brake() {
  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
  digitalWrite(out3, LOW);
  digitalWrite(out4, LOW);
}


int getAngle() {
  axis_X = Wire.read() << 8 | Wire.read();
  axis_Y = Wire.read() << 8 | Wire.read();
  axis_Z = Wire.read() << 8 | Wire.read();

  int xAng = map(axis_X, minVal, maxVal, -90, 90);
  int yAng = map(axis_Y, minVal, maxVal, -90, 90);
  int zAng = map(axis_Z, minVal, maxVal, -90, 90);

  return RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); // convert radians to degree
}

void turn(int angle, char dir) {
  if ( dir == 'l') {
    serv.writeMicroseconds(1500 - var) //fill in var
  }
  if ( dir == 'r') {
    serv.writeMicroseconds(1500 + var) //fill in var
  }
  int initAngle, currAngle  = getAngle();
  while (abs(currAngle - initAngle) < angle) {
    currAngle = getAngle();
    drive('f', 255); //Change speed if too fast
  }
  serv.writeMicroseconds(1500);
}

void drive(char dir, int spd = 0) {
  motor_in = {        //set motor input data based on desired direction
    dir == 'f' || dir == 'c', //in1 is 1 when forward or coast, and 0 otherwise
    dir == 'r' || dir == 'c'  //in2 is 1 when reverse or coast, and 0 otherwise
  }
  if ( dir == 'f') {
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
    analogWrite(enabler1, spd);

    digitalWrite(out3, HIGH);
    digitalWrite(out4, LOW);
    analogWrite(enabler2, spd);
  }
  if (dir == 'r') {
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);
    analogWrite(enabler1, spd);

    digitalWrite(out3, LOW);
    digitalWrite(out4, HIGH);
    analogWrite(enabler2, spd);

  }

}
void setup() {
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  pinMode(enabler1, OUTPUT);
  pinMode(enabler2, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  serv.attach(1); //TODO: attach servo pin
  // LED_Bulletin
  pinMode(LED_BUILTIN, OUTPUT);
  // Gyro Sensor
  Wire.begin();
  Wire.beginTransmission(MPU_addr);

  Wire.write(0x6B); // Put Gyro Sensor to Sleep
  Wire.write(0); // Turn on Gryo Sensor
  Wire.endTransmission(true);      // End Transmission
}

void loop() {
  Wire.beginTransmission(MPU_addr); //
  Wire.write(0x3B);  // Then start with register 0x3B (ACCEL_XOUT_H)
  Wire.requestFrom(MPU_addr, 14, true); // Request data from 14 registers
  valIr1 = analogRead(ir1); 
  valIr2 = analogRead(ir2); 
  valIr3 = analogRead(ir3); 
  valIr4 = analogRead(ir4);  
  int thres; //threshold for value of reflective tape 
  axis_X = Wire.read() << 8 | Wire.read();
  axis_Y = Wire.read() << 8 | Wire.read();
  axis_Z = Wire.read() << 8 | Wire.read();

  int xAng = map(axis_X, minVal, maxVal, -90, 90);
  int yAng = map(axis_Y, minVal, maxVal, -90, 90);
  int zAng = map(axis_Z, minVal, maxVal, -90, 90);

  int x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); // convert radians to degree
  int pos = map(x, 0, 270, 0, 270); // if you want to convert 0 to 360 deg to 0 to 270
`
  serv.write(pos); // set servo to angle pos

  while (true){
    drive('f', 255); 
    if ( valIr1 > thres){
      while(valIr1 > thres){
        turn('r', 45);
        digitalWrite(LED_BUILTIN, HIGH);   
        delay(1000);                       
        digitalWrite(LED_BUILTIN, LOW);    
        delay(1000);  
      } 
      resetServ(); 
     if ( valIr2 > thres){
      while(valIr2 > thres){
        turn('l', 45);
        digitalWrite(LED_BUILTIN, HIGH);   
        delay(1000);                       
        digitalWrite(LED_BUILTIN, LOW);    
        delay(1000);  
      } 
      resetServ();    
    }
  }
}

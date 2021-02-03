#include <Wire.h>
#include <Servo.h>
#include <Encoder.h>
int in1, in2, in3, in4;    //motor controller input pins
int out1, out2, out3, out4;
int enabler1, enabler2;   //motor controller enable pins
int ir1, ir2, ir3, ir4; // ir1 and 2 are top left and right, ir3 and 4 are bottom left and right
Servo serv;
Encoder enc(2, 3);
// Ultrasonic Sensor
HCSR04 hc(2,3);
// Encoder
Encoder enc(2, 3);

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

//original
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
//updated
void turnUpdated(int angle, char turnDir, char driveDir){ 
      
      int initAngle,currAngle  = getAngle(); 
        while (abs(currAngle - initAngle) < angle){
          if ( turnDir == 'l'){
            serv.writeMicroseconds(1500 - var) //fill in var
          }
          if ( turnDir == 'r'){
            serv.writeMicroseconds(1500 + var) //fill in var
          }
          currAngle = getAngle();
          if( driveDir == 'f'){
           forward(255); //Change speed if too fast
          }else if ( driveDir == 'r'){
           reverse(255);                                     
          }
          //Need to test ^
        }
      brake();
      serv.writeMicroseconds(1500); 
}
void turn(char dir){
  if ( turnDir == 'l'){
        serv.writeMicroseconds(1500 - var) //fill in var
   }
   if ( turnDir == 'r'){
       serv.writeMicroseconds(1500 + var) //fill in var
  }
}
void moveDistance(long dist, char dir) {
  enc.write(0); // initizliate encoder to 0
  bool move = true;
  while (move) {
    long encoder = enc.read();
    encoder *= conversion // TODO: figure out conversion
    while ( encoder < dist) {
      if (dir == 'f') {
        drive(dir, 255);
      }
      if (dir == 'r') {
        drive(dir, 255);
      }
    }
    move = false;
    //TODO: Make function for encoder
  }
}

/*
    Drives the car using the motor controller commands
    Inputs:
      [char] dir: motor direction commands
        {'b': brake, 'r': reverse, 'f': forward, 'c': coast}
      [int] speed: motor speed (full speed is 255)
        {0 <= speed <= 255}
*/

void drive(char dir, int spd = 0) {
  motor_in = {        //set motor input data based on desired direction
    dir == 'f' || dir == 'c', //in1 is 1 when forward or coast, and 0 otherwise
    dir == 'r' || dir == 'c'  //in2 is 1 when reverse or coast, and 0 otherwise
  }
  if (motor_in[0] == 1 && motor_in[1] == 0 ){
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
    analogWrite(enabler1, spd);

    digitalWrite(out3, HIGH);
    digitalWrite(out4, LOW);
    analogWrite(enabler2, spd);
  }
  else if (motor_in[0] == 0 && motor_in[1] == 1)  {
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);
    analogWrite(enabler1, spd);

    digitalWrite(out3, LOW);
    digitalWrite(out4, HIGH);
    analogWrite(enabler2, spd);
    
  }else if(motor_in[0] == 1 && motor_in[1] == 1){
    digitalWrite(out1, HIGH);
    digitalWrite(out2, HIGH);
    digitalWrite(out3, HIGH);
    digitalWrite(out4, HIGH);
  }else 
    brake()
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

  axis_X = Wire.read() << 8 | Wire.read();
  axis_Y = Wire.read() << 8 | Wire.read();
  axis_Z = Wire.read() << 8 | Wire.read();

  int xAng = map(axis_X, minVal, maxVal, -90, 90);
  int yAng = map(axis_Y, minVal, maxVal, -90, 90);
  int zAng = map(axis_Z, minVal, maxVal, -90, 90);

  int x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); // convert radians to degree
  int pos = map(x, 0, 270, 0, 270); // if you want to convert 0 to 360 deg to 0 to 270

  serv.write(pos); // set servo to angle pos
}

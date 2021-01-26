
#include <Wire.h>
#include <Servo.h>
#include <Encoder.h>
#include <HCSR04.h>
int in1, in2, in3, in4;    //motor controller input pins
int out1, out2, out3, out4;
int enabler1, enabler2;   //motor controller enable pins
int ir1, ir2, ir3, ir4; // ir1 and 2 are top left and right, ir3 and 4 are bottom left and right
Servo serv;
// Ultrasonic Sensor
HCSR04 hc(2,3);
// Encoder
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

  // Gyro Sensor
  Wire.begin();
  Wire.beginTransmission(MPU_addr);

  Wire.write(0x6B); // Put Gyro Sensor to Sleep
  Wire.write(0); // Turn on Gryo Sensor
  Wire.endTransmission(true);      // End Transmission
}

void loop() {'
  drive('f',100);
  if (hc.dist() < 400){
    brake(); 
  }
  moveDistance(100,'f');
}

#include <Servo.h>
#include <Wire.h>  
#include <Encoder.h>
  const int MPU_addr=0x68; // I2C address of the MPU6050
  int16_t axis_X,axis_Y,axis_Z; // Three variables to store x,y,and z axis 
  int minVal=265; // minimum and maximum value is set as 265 and 402 for measuring angle from 0 to 360
  int maxVal=402; 
  int out1; 
  int out2;
  int out3; 
  int out4; 
  int enabler1;
  int enabler2;
  Servo serv;
  Encoder enc(2, 3);
   
  
    
   
void forward( int spd){
  digitalWrite(out1, HIGH);  
  digitalWrite(out2, LOW);  
  analogWrite(enabler1, spd);

  digitalWrite(out3, HIGH);  
  digitalWrite(out4, LOW);  
  analogWrite(enabler2, spd);
}
void brake(){
    digitalWrite(out1, LOW);  
    digitalWrite(out2, LOW);  
    digitalWrite(out3, LOW);  
    digitalWrite(out4, LOW);  
     
}

void reverse( int spd){
  digitalWrite(out1, LOW);  
  digitalWrite(out2, HIGH);  
  analogWrite(enabler1, spd);

  digitalWrite(out3, LOW);  
  digitalWrite(out4, HIGH);  
  analogWrite(enabler2, spd);
}
int getAngle(){
      axis_X=Wire.read()<<8|Wire.read();
      axis_Y=Wire.read()<<8|Wire.read();
      axis_Z=Wire.read()<<8|Wire.read();
    
      int xAng = map(axis_X,minVal,maxVal,-90,90);
      int yAng = map(axis_Y,minVal,maxVal,-90,90);
      int zAng = map(axis_Z,minVal,maxVal,-90,90);
  
      return RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); // convert radians to degree   
}

void turn(int angle, char turnDir, char driveDir){ 
      if ( turnDir == 'l'){
        serv.writeMicroseconds(1500 - var) //fill in var
      }
      if ( turnDir == 'r'){
        serv.writeMicroseconds(1500 + var) //fill in var
      }
      int initAngle,currAngle  = getAngle(); 
        while (abs(currAngle - initAngle) < angle){
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

void moveDistance(long dist, char dir){
  enc.write(0); // initizliate encoder to 0
  bool move = true; 
  while (move){
  long encoder = enc.read();
  encoder *= conversion // TODO: figure out conversion
    while ( encoder < dist){
      if (dir == 'f'){
        forward(255); 
      }
      if (dir == 'r'){
        reverse(255); 
      }  
    }
    move = false; 
  
  }
  
  
}

void setup(){
  pinMode(out1, OUTPUT); 
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT); 
  pinMode(out4, OUTPUT);
  pinMode(enabler1, OUTPUT);  
  pinMode(enabler2, OUTPUT);  
  serv.attach(1); //TODO: attach servo pin

  // Gyro Sensor
  Wire.begin();            
  Wire.beginTransmission(MPU_addr);  

  Wire.write(0x6B); // Put Gyro Sensor to Sleep                    
  Wire.write(0); // Turn on Gyro Sensor
  Wire.endTransmission(true);      // End Transmission
}


void loop() {
    // Reverse Speed = 255, no Speed = 0
    /* 
    Enabler In2 In1
       X 0 0 = Braking
       1 0 1 = Full Forward
       1 1 0 = Full Backward  
       X 1 1 = Float

       For controlling speed of motors, use analogWrite(pin, value);    

       Servo Position (writeMicroseconds)
    */
    Wire.beginTransmission(MPU_addr); // 
    Wire.write(0x3B);  // Then start with register 0x3B (ACCEL_XOUT_H)   
    Wire.requestFrom(MPU_addr,14,true); // Request data from 14 registers

    axis_X=Wire.read()<<8|Wire.read();
    axis_Y=Wire.read()<<8|Wire.read();
    axis_Z=Wire.read()<<8|Wire.read();
    
    int xAng = map(axis_X,minVal,maxVal,-90,90);
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);

    int x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); // convert radians to degree
    int pos = map(x,0,270,0,270); // if you want to convert 0 to 360 deg to 0 to 270
    
    serv.write(pos); // set servo to angle pos

    /* while -x <= 90: 
     *  // Turn left 
     *  axis_X=Wire.read()<<8|Wire.read();
        axis_Y=Wire.read()<<8|Wire.read();
        axis_Z=Wire.read()<<8|Wire.read();
        int x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); // convert radians to degree
     *  
     *  
     *  forward 90 degrees
     *  reverse 90 degrees in the opposite direction
     *  
     *  
     */
     /* Initial Encoder Value */



     //Need to pull up to the front car (rears need to be inline with one another)
     moveDistance( 3,'f'); 
     //turn the car to fit into the parking slot 
     turn(-35, 'r', 'r');
     //Go back a distance of X
     moveDistance(2,'r');
     //realign the car parallel to curb
     turn(0,'l','r');
     //Wire.write(0);
     

}

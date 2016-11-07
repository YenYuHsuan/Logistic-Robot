/*
*******NCU Researcher*******
******* Yen Yu Hsuan ******* 
****************************
This project combine all of my little project that i have done before.

First of all, Introduce the Hardwares

<H1> BT module : HC-06
1. HC-06 Datasheet:http://www.buildbot.com.br/files/ModuloBluetooth-HC06.pdf
   HC -06:Bluetooth serial interface module,
   is used for converting serial port to Bluetooth.

2. circuit:
HC-06 -> Arduino
----------------
Vcc   ->   5V
GND   ->   GND
TX    ->   RX1(19)
RX    ->   TX1(18)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<H2>DC Motor(with encoder) & L298N module
1. DC motor Datasheet:https://goo.gl/bJ1t7w

2. L298N Datasheet:https://goo.gl/kmFkbQ
   L298N is the Dual H-Bridge motor controller IC
   it can make arduino to control motor direction and speed

3. circuit:
DC motor  <-> L298N <-> Arduino
--------------------------------
motor1+   <-> OUTA  ///////////
motor1-   <-> OUTB  ///////////
encoder1A <----------> D26
encoder1B <----------> D27
motor2+   <-> OUTC  ///////////
motor2-   <-> OUTD  ///////////
encoder2A <----------> D28
encoder2B <----------> D29
//////////   INA <---> D22
//////////   INB <---> D23
//////////   INC <---> D24
//////////   IND <---> D25
//////////   ENA <---> PWM2
//////////   ENB <---> PWM3

Note:
MEGA has 52 digital pins,
15 of them is PWM(Pulse Width Modulation) pins
MEGA PWM pins: 2 to 13 and 44 to 46. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<H3>Robot arm
Using Servo (model: SG90) to control robot arm

1. SG90 Datasheet:http://goo.gl/ivhlqL
   By Using Servo Example & Library:
   'Sweep': http://arduino.cc/en/Tutorial/Sweep
   'writeMicroseconds': https://www.arduino.cc/en/Reference/ServoWriteMicroseconds
   
2. robot arm's body is 3D printed by using the module made by Kwendenarmo 
   here is the sourse:http://www.thingiverse.com/thing:436096

3. circuit:
Servo   <-----> Arduino
-----------------------
S1.Vcc  <----->  5V
S2.Vcc  <----->  5V
S3.Vcc  <----->  5V
S1.GND  <----->  GND
S2.GND  <----->  GND
S3.GND  <----->  GND
S1.PWM  <----->  PWM4
S2.PWM  <----->  PWM5
S3.PWM  <----->  PWM6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
//////////////////////////////include Library///////////////////////////////
#include <Servo.h>

///////////////////////////////function List////////////////////////////////
/*motor control*/
void action(char direction);
void motorstop(byte flag, byte motorspeed);
void forward(byte flag, byte motorspeed);
void backward(byte flag, byte motorspeed);
void left(byte flag, byte motorspeed);
void right(byte flag, byte motorspeed);

/*robot arm*/
void robot_arm(char levelnumber);     // robot arm main function
void arm_go(int level[2]);            // robot arm go to certen position
void pickup();                        // close the clips to pick things

//////////////////////////define global variables///////////////////////////

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 2;  //PWM
int in1 = 22;
int in2 = 23;
const int encoderApin1 = 26;                 
const int encoderApin2 = 27;
// motor two
int enB = 3;  //PWM
int in3 = 24;
int in4 = 25;
const int encoderBpin1 = 28;                 
const int encoderBpin2 = 29;

// create servo object to control a servo 
Servo myservo1,myservo2,myservo3;
const int servo1 = 0,servo2 = 1;  
const int inipos[2] = {50, 0};    //initial position   unit:*30ms
const int endpos[2] = {50,85};    //end position
int arm_pos [2] = {50, 0};        // variable to store the servo position

//////////////////////////END define global variables///////////////////////

void setup() {
  pinMode(13,OUTPUT);   // To show setup progress
  digitalWrite(13,HIGH);// Setup start
  
  // initialize both serial ports:
  Serial.begin(9600);   //TX0,RX0 begin 
  
  // Set Serial one be BlueTooth pins
  Serial1.begin(9600);  //TX1,RX1 begin

  // Setup motor controller pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // attaches the servos on pin 4,5,6 to the servo object
  myservo1.attach(4);  
  myservo2.attach(5);
  myservo3.attach(6);
  // initialize robot arm               
  myservo1.writeMicroseconds(1500);     //  ＞０－０         Note: ０ is servo motor
  myservo2.writeMicroseconds(0);        //        ｜               ＞ is clips
  myservo3.writeMicroseconds(0);        //   -----０-----         --- is ground
  
  
  digitalWrite(13,LOW);  // Setup end
}

void loop() {
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if (Serial1.available()) {  // if BT read value
          char  var= Serial1.read();
          Serial.write(var);        // show to USB
          
          // Set condition for motor control
          if(var=='f'||var=='b'||var=='l'||var=='r'||var=='s')action(var);
          
          // Set condition for robot arm
          else if(var=='P'){ 
                delay(500);
                char level=Serial1.read();
                if(level>47&&level<58){   // level is number ( 0 ~ 9 ) only
                  Serial.println(level);
                  robot_arm(level);       // read level value from BT
                }
           }
    }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MEGA~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ 
   if (Serial.available()) {  // if USB read value
          char var = Serial.read();
          Serial1.write(var);     // show to BT
           
            // Set condition for motor control
          if(var=='f'||var=='b'||var=='l'||var=='r'||var=='s')action(var);
            
            // Set condition for robot arm
          else if(var=='P'){ 
                char level=Serial.read();
                if(level>47&&level<58){   // level is number ( 0 ~ 9 ) only
                   Serial1.println(level);
                   robot_arm(level);       // read level value from USB
                }
          }
   }
}
///////////////////////////////motor control////////////////////////////////
void action(char dir)
{
  switch(dir)
    {
      case 'f':   // car forward
                forward(0, 255);
               // encoder(100000);
                break;
                
      case 'b':   // car backward            
                backward(0, 255);
              //  encoder(100);               
                break;
                
      case 'l':   // car turn left
                left(0, 200);
               // encoder(50);
                break;
                
      case 'r':   // car turn right
                right(0, 200);
               // encoder(6);
                break;
                
      case 's':   // car stop
                motorstop(0, 0);
                break;
    }   
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~each direction~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void motorstop(byte flag, byte motorspeed)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}

void forward(byte flag, byte motorspeed)
{
  Serial.println("Forward!");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
  
}

void backward(byte flag, byte motorspeed)
{
  Serial.println("Backward!");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
   
}

void right(byte flag, byte motorspeed)
{
  Serial.println("Turn Right! ");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);  
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
  
}

void left(byte flag, byte motorspeed)
{
  Serial.println("Turn Left!");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}
/////////////////////////////////robot arm//////////////////////////////////
void robot_arm(char levelnum)  // robot arm main function
{
 // each level has different value
 // level = {servo1,servo2}  unit: *30(ms)
 int level1[2]    = {73,25};
 int level2[2]    = {50, 0};
 int level3[2]    = {60,50}; 
 int level_ini[2] = {inipos[servo1],inipos[servo2]};
 int level_end[2] = {endpos[servo1],endpos[servo2]}; 
 
 switch(levelnum) //select the level
 {
    case '1':
       arm_go(level1);
       break;
    case '2':
       arm_go(level2);
       break;
    case '3':
       arm_go(level3);
       break;
 }
  pickup();
  backward(0,200);
  delay(1000);
  motorstop(0,0);
  arm_go(level_end);  // go to end position  (endpos) ,and put down
  arm_go(level_ini);  // go to home position (inipos)
  forward(0,200);
  delay(500);
  motorstop(0,0);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~each function~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void arm_go(int level[2])  // let robot arm go to certen position
{
  /* initial position , unit: *30(ms)
  inipos[2] = {50, 0};
  endpos[2] = {50,85};
  */
    if(level[servo2]>=arm_pos[servo2])
      for(int i=arm_pos[servo2];i<level[servo2];i++){
          myservo2.writeMicroseconds(i*30);
          delay(40);}
    else
    {
      for(int i=arm_pos[servo2];i>level[servo2];i--){
          myservo2.writeMicroseconds(i*30);
          delay(40);}
    }

    if(level[servo1]>=arm_pos[servo1])
       for(int i=arm_pos[servo1];i<level[servo1];i++){
           myservo1.writeMicroseconds(i*30);
           delay(40);}
     else
     {
       for(int i=arm_pos[servo1];i>level[servo1];i--){
           myservo1.writeMicroseconds(i*30);
           delay(40);}
     }
   
   //store the position value
   arm_pos[0]=level[0];
   arm_pos[1]=level[1];
   delay(100);

   // if the position is at endpos then open the clips (put down)
   if(level[0]==endpos[0]&&level[1]==endpos[1]) 
   myservo3.writeMicroseconds(0); //put down
}

void pickup()  // close the clips to pick things
{
  delay(400);
  for(int i=0;i<180;i+=3){
  myservo3.write(i);
  delay(15);
  }
  delay(400);
}


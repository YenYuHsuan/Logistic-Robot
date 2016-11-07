/* 
*********Researcher*********
******* Yen Yu Hsuan ******* 
****************************
 By Using Example 'Sweep'
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h>  

// create servo object to control a servo 
Servo myservo1,myservo2,myservo3;  
const int servo1 = 0,servo2 = 1; 
const int inipos[2] = {50, 0};
const int endpos[2] = {50,85};     
int arm_pos [2] = {50, 0};    // variable to store the servo position

void setup() 
{ 
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  
  // attaches the servo on pin 9 to the servo object
  myservo1.attach(4);  
  myservo2.attach(5);
  myservo3.attach(6);
  
  Serial.begin(9600);
  ///////////robot arm setup/////////////
  myservo3.writeMicroseconds(0);
  myservo1.writeMicroseconds(1500);
  myservo2.writeMicroseconds(0);
  //arm_initialize();
  ///////////////////////////////////////
  
  digitalWrite(13,LOW);

}
void loop() 
{ 
  if(Serial.available()>0)
  {
    char level= Serial.read();
    if(level>47&&level<58){  //level has to be number
    Serial.println(level);
    robot_arm(level);  //read level value from MEGA
    Serial.println("Done");  //return 'c' to MEGA to continue
    }
  }
} 

void arm_home()
{
   for(int i=endpos[servo1];i>inipos[servo1];i--){
    myservo1.writeMicroseconds(i*30);
    delay(40);
   } 
   for(int i=endpos[servo2];i>inipos[servo2];i--){
    myservo2.writeMicroseconds(i*30);
    delay(40);
   }
   arm_pos[0]=inipos[0];
   arm_pos[1]=inipos[1];
   myservo3.writeMicroseconds(0);
}

void arm_go(int level[2])
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
   if(level[0]==endpos[0]&&level[1]==endpos[1])
   myservo3.writeMicroseconds(0);//put down
}

void pickup()
{
  delay(400);
  for(int i=0;i<180;i+=3){
  myservo3.write(i);
  delay(15);
  }
  delay(400);
}

void robot_arm(char levelnum)
{
 // each level has different value
 // level = {servo1,servo2}  unit: *30(ms)
 int level1[2]    = {73,25};
 int level2[2]    = {50, 0};
 int level3[2]    = {60,50}; 
 int level_end[2] = {50,85}; 
 switch(levelnum)
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
  arm_go(level_end);
  arm_home();
}

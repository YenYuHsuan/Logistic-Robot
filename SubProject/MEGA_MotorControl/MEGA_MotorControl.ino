/*
*********Researcher*********
******* Yen Yu Hsuan ******* 
****************************

DC Motor(with encoder) & L298N module
1. DC motor Datasheet:https://goo.gl/bJ1t7w

2. L298N Datasheet:https://goo.gl/kmFkbQ
   L298N is the Dual H-Bridge motor controller IC
   it can make arduino to control motor direction and speed

3. circuit:
DC motor <-> L298N <-> Arduino
----------------------------
motor1+  <-> OUTA  ///////////
motor1-  <-> OUTB  ///////////
encoder1A <----------> D26
encoder1B <----------> D27
motor2+  <-> OUTC  ///////////
motor2-  <-> OUTD  ///////////
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
 */
 
// connect motor controller pins to Arduino digital pins
// motor one
int enA = 2;
int in1 = 22;
int in2 = 23;
const int encoderApin1 = 26;                 
const int encoderApin2 = 27;
// motor two
int enB = 3;
int in3 = 24;
int in4 = 25;


void setup() {
  // initialize both serial ports:
  Serial.begin(9600);   //TX0,RX0 begin 
  
  //Let Serial one be BlueTooth pins
  Serial1.begin(9600);  //TX1,RX1 begin
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
}

void loop() {
  //define USB port    (TX0,RX0) as port 0
  //define BlueTooth   (TX1,RX1) as port 1
  
  // read from port 1, send to port 0:
  if (Serial1.available()) {  //if BT read value
    char Direction = Serial1.read();
    Serial.write(Direction);
    action(Direction);
    if(Direction=='P'){
      delay(2000);
      int D=Serial1.read();
      Serial.write(D);}
    Serial.flush();
  }
  
  // read from port 0, send to port 1:
  if (Serial.available()) {   //if USB port read value
    int Direction = Serial.read();
    Serial1.write(Direction);
    action(Direction);
  }
     
}

void encoder(int count_max) //encoder feedback function
{
  const int step_unit=10000;//each step unit consist with 1000 up High(pin1) and Low(pin2)
  
  int encoder_count=0; //initialize the encoder count to 0
  
  delay(50); //avoid over counting in the begining (detect intrupt)
  
    while(1){
        if(digitalRead(encoderApin1)==HIGH){
         if(digitalRead(encoderApin2)==LOW){
            if(encoder_count==count_max){
             motorstop(0,0);
             break;
             }
             encoder_count++;
          }//end encoderApin2
        }//end encoderApin1
   }
}

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

/////////////////////////each direction///////////////////////////////////
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


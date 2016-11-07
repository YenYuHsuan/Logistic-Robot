/*
*********Researcher*********
******* Yen Yu Hsuan ******* 
****************************
BT hardware : HC-06
HC -06:Bluetooth serial interface module,
is used forconverting serial port to Bluetooth.

circuit:
HC-06 ->  MEGA
Vcc   ->   5V
GND   ->   GND
TX    ->   RX
RX    ->   TX

HC-06 Datasheet:http://www.buildbot.com.br/files/ModuloBluetooth-HC06.pdf
 */


void setup() {
  // initialize both serial ports:
  Serial.begin(9600);   //TX0,RX0 begin 
  
  //Let Serial one be BlueTooth pins
  Serial1.begin(9600);  //TX1,RX1 begin
}

void loop() {
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
  
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte);
  }
}

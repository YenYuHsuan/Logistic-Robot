/*
  Mega multple serial test
 
 Receives from the main serial port, sends to the others. 
 Receives from serial port 1, sends to the main serial (Serial 0).
 
 This example works only on the Arduino Mega
 
 The circuit: 
 * Any serial device attached to Serial port 1
 * Serial monitor open on Serial port 0:
 
 created 30 Dec. 2008
 modified 20 May 2012
 by Tom Igoe & Jed Roach
 
 This example code is in the public domain.



*********Researcher*********
******* Yen Yu Hsuan ******* 
****************************
MEGA has 4 sets of TX,RX [TX0,RX0] -> [TX3,RX3])

It is corresponded as
TX0,RX0 -> Serial     [ 1, 0]
TX1,RX1 -> Serial1    [18,19]
TX2,RX2 -> Serial2    [16,17]
TX3,RX3 -> Serial3    [14,15]

Note:
*Pins 0 and 1 are also connected to the corresponding pins 
of the ATmega16U2 USB-to-TTL Serial chip.
So before upload code ,make sure Pin 0 and 1 don't have any plugins.

* Do not use SoftwareSerial to define each pins.
There'll be some erro when reading or sending signal.
 */


void setup() {
  // initialize both serial ports:
  Serial.begin(9600);   //TX0,RX0 begin
  
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

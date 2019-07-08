//Edge code
//Group #2: Luis Jimenez, Reynaldo Del Rio, Carolina Vazquez, Shahvez Mehdi 
//IoT Final Project 

#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 

int InputDegrees; 
bool result; 

const byte ReceiverAddress[5] = {'2', 'L', 'A', 'R', 'C'}; //address of the receiver 
 
RF24 radio(9, 10);// initializs radio variable, module is connected to pins 9 and 10 
 
char dataToSend[60]; //the command/data you need to send to the receiver with a size of 42 characters 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  radio.begin(); //radio module begins      
  radio.setDataRate( RF24_250KBPS ); //set the radio data rate to be 250 Kbps     
  radio.setRetries(3,5); // delay, count     
  radio.openWritingPipe(ReceiverAddress); //specify the address of the receiver 
}

void loop() {
  // put your main code here, to run repeatedly:

//we will provide the user with a list of possible commands for them to pick 
Serial.println("Here is a list of commands:");
Serial.println("Type either 1, 2, or 3");
Serial.println("1. Read the value of the photo sensor");
Serial.println("2. Move the servo motor forward");
Serial.println("3. Move the servo motor backward");

while(Serial.available()==0);

int command = Serial.parseInt();// stores 1, 2, or 3 depending on what command the user picks
Serial.end();
Serial.begin(9600);


if(command == 1)
{
  String c1 = "Read the value of the photo sensor ";
  c1.toCharArray(dataToSend, c1.length());// changes to String command into char array 
  result = radio.write( &dataToSend, sizeof(dataToSend) ); // stores the return value from receiver class 
  //Serial.println(result);//prints result
}

if(command == 2 || command == 3)//checks whether user picks command 2 or 3 
{
     
   Serial.println("Please enter degrees: ");//asks user for degrees to move servo
   while(Serial.available()==0);
   InputDegrees = Serial.parseInt(); // stores the degrees the user picks 
   Serial.end();
   Serial.begin(9600);

   String X = String(InputDegrees);//converts int into string 


   if(command == 2)
   {
     String c2 = "Rotate servo + " + X + " degrees ";
     c2.toCharArray(dataToSend, c2.length());//stores c2 as char array 
     result = radio.write( &dataToSend, sizeof(dataToSend) );// stores the result from receiver method 
    
   }
   if(command == 3)
   {
     String c3 = "Rotate servo - " + X + " degrees ";
     c3.toCharArray(dataToSend, c3.length());//stores c3 as char array 
     result = radio.write( &dataToSend, sizeof(dataToSend) );// stores the result from receiver method 
   }

}


   Serial.print("Sending instruction: ");     
   Serial.println(dataToSend);//ask the radio module to send the string defined     
      
 
    if (result) {         
      Serial.println(" Data sent and an acknowledge is received");     
    }     
    else {         
      Serial.println(" transmission is failed");     
    } 
 
    delay(2000); 
}

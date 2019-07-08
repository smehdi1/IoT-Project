//Thing Code
//Group #2: Luis Jimenez, Reynaldo Del Rio, Carolina Vazquez, Shahvez Mehdi 
//IoT Final Project 

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 

 
//this is the address of the the receiver (the thing who will run this program) 
const byte MyAddress[5] = {'2','L','A','R','C'};   

//initialize a Radio variable, this module is connected to pins 9 and 10 
RF24 radio(9, 10);     
 
char dataReceived[60];//this is buffer to receive the sent data from the sender 
bool newData = false; 

int value; //photosensor int 
int n = 1;//buzzer

//servo values
int beginIndex;//initial index of degree command
int endIndex;//last index of degree command
String degree = "";
int d;//stores degree string as integer 



void setup() {
  myservo.attach(6);//attaches the servo on pin ~3 to the servo object 
  pinMode(A0, INPUT);// photoresistor 
  pinMode(5, OUTPUT);// Buzzer is connected to pin 5
  Serial.begin(9600); 
 
  radio.begin(); //begin the radio module     
  radio.setDataRate( RF24_250KBPS );    //set the radio data rate to be 250 Kbps          
    
  //specify your address to intercept the received packets addressed to me     
  radio.openReadingPipe(1, MyAddress);            
  radio.startListening();   //start listening the radio packets sent from nearby things 
  myservo.write(0); // initiate servo at 0
        
  
}

void loop() {
  int pos = 0;
  if (radio.available() ) { //i received command on radio 
    //read the received data and store it in the buffer defined "dataReceived"         
    radio.read(&dataReceived, sizeof(dataReceived) );      
 
    Serial.print("Data received ");         
    Serial.println(dataReceived); //print the received data


   
    //strings to compara data received; used in "If" statements 
    String data(dataReceived);
    String c1 = "Read the value of the photo";
    String c2 = "Rotate servo +";
    String c3 = "Rotate servo -";

    if (data.startsWith(c1)){//prints photosensor data
      
      //buzzer beeps once if command 1 
      if(n < 2){//buzz once 
      for(int i = 1; i < 2; i++){
        tone(5,5000);
        delay(500);
        noTone(5);
        delay(500);
      }
      n = 1; 
    }
      Serial.println("I received the first command");
      value = analogRead(A0); 
      Serial.println(value);//return photosensor value 
      
    }
   //command two 
    else if(data.startsWith(c2)){ //move servo + degrees
     
     //buzzer beeps twice if command 2
      if(n < 2) { //buzz twice
        for(int i = 1; i < 3; i++){
          tone(5,5000);
          delay(500);
          noTone(5);
          delay(500);
        }
        n = 1;
      }

      //servo
      beginIndex = data.indexOf('+');
      endIndex = data.lastIndexOf(' ');
      degree = data.substring(beginIndex + 2, endIndex);
      d = degree.toInt();
      pos = pos + d;// update servo to move forward 
      
      if(pos > 180) { pos = 180;}// position cannot go beyond 180 degrees
      if(pos <= 180) { 
        //print comments for servo's position
        //Serial.print("Current position: ");
        //Serial.print(pos);
        
        myservo.write(pos);// tell servo to go to position in variable 'pos' */
        delay(15);                       
      }                 
      
      Serial.println("I received the second command");
      
    }
    
    //Command three
    else if(data.startsWith(c3)){ 
      
      //buzzer beeps three times if command 3
      if(n < 2) { //buzz three times
        for(int i = 1; i < 4; i++){
          tone(5,5000);
          delay(500);
          noTone(5);
          delay(500);
        }
        n = 1;
      }

      //Servo 
      beginIndex = data.indexOf('-');
      endIndex = data.lastIndexOf(' ');
      degree = data.substring(beginIndex + 2, endIndex);
      d = degree.toInt();
      pos = pos - d; //update servo to move backwards
      
      if(pos < 0) { pos = 0;}//servo cannot go negative degrees
      
      if(pos >= 0) {  
        //print comments for servo's position
        //Serial.print("Current position: ");
        //Serial.print(pos);
        
        myservo.write(pos);// tell servo to go to position in variable 'pos' */
        delay(15);         
      }
      Serial.println("I received the third command");
    }
    
    //No command or no connected
    else{
      Serial.println("unsupported");
    }

    Serial.flush();
    data = "";
  
  }
  delay(2000);

   

}

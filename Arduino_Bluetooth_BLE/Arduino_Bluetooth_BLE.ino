#include <SoftwareSerial.h>
//This is a sketch for chatting with a Phone or tablet.
//It will get a command to turn the light on or a seperate command to turn it off.
//AT commands can be sent through the IDE Monitor command. Or just data can be sent back to the phone.
SoftwareSerial BLE_Serial(3, 4); // RX, TX
char inData[64]; // Much larger than any expected command -  birrr
int i=0;//
int index=0;//This would is used as the pointer to where in the array the data stream is presently
char inChar=-1;//This is a base char, necessary later for transparency
bool command=false;//This is a flag noting if a command just finished
void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  BLE_Serial.begin(9600);//The default baudrate for the module is 9600
  pinMode(9, OUTPUT);//This for the light
  digitalWrite(9, LOW);//Double check that the light is off.
}

void loop() // run until the end of time
{
  if(BLE_Serial.available()) //Check to see if any data has streamed into the module
  {
    byte byte_count=BLE_Serial.available();//Often one byte comes in, but sometimes more.
    for(i=0;i<byte_count;i++)//Handle the number of incoming bytes
    {
      inChar=BLE_Serial.read();//Read one byte
      if(index>63){index=0;}//Don't allow buffer overruns.
      if(inChar==10) //Command terminator detected
      {
        inData[index]='\0';//Close off the string
        command=true;//This is a 1.0 command
        index=0;//reset 
      }
      else
      {
        inData[index]=inChar;//shove the next character into the array
        index++;
      }
    }
    if(command==true && String(inData)=="INCOMING SIGNAL")//Let's turn on the light
    {
      command=false;//Reset for the next command
      digitalWrite(9, HIGH);//Turn on the really bright light
      Serial.println(String(inData));//Write out the assembled command
    }
    else if(command==true && String(inData)=="ASSISTANCE eN ROUTE")//Let's start flashing.
    {
      command=false;//Reset for the next comment
      digitalWrite(9, LOW);//Blessed darkness
      Serial.println(String(inData));//Write out the assembled command
    }
  }
  if(Serial.available())//When stuff is typed in the Arduino monitor window it can send it up to the computer
  {
    BLE_Serial.write(Serial.read());//Read it and pass it along.
  }
}

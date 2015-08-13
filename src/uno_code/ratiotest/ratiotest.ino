#include <Servo.h>

Servo left;
Servo right;
char command;

void setup (){
  left.attach(9);
  right.attach(10);
  Serial.begin(9600);
}

void loop(){
  if (Serial.available())
  {
    command=Serial.read();
    Serial.println(command);
    
    if (command =='a')
    {
      mode(140,87.5,250);
      mode(90,90,1000);
    }
    else if (command =='b')
    {
      mode(140,86.66,250);
      mode(90,90,1000);     
    }
    else if (command=='c')
    {
      mode(140,85,250);
      mode(90,90,1000);      
    }
    else if (command=='d')
    {
      mode(140,80,250);
      mode(90,90,1000);      
    }
    
  }

}

void mode(int leftSpeed, int rightSpeed, int time)
{
  left.write(leftSpeed);
  right.write(rightSpeed);
  delay(time);
}




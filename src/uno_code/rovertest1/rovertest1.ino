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
    
    if (command =='l')
    {
      mode(100,90,1000);
    }
    else if (command =='r')
    {
      mode(90,100,1000);
    }
    else if (command=='s')
    {
      mode(90,90,1000);
    }
    else if (command=='d')
    {
      mode(60,60,100);
    }
    
  }

}

void mode(int leftSpeed, int rightSpeed, int time)
{
  left.write(leftSpeed);
  right.write(rightSpeed);
  delay(time);
}




#include <ros.h>
#include <std_msgs/Float32.h>
#include <Servo.h>

//#define DIR_R  7             //Right motor Direction control line
//#define DIR_L  8             //Left motor Direction control line
//#define PWM_R  9             //Right motor PWM control line
//#define PWM_L  10            //Left motor PWM control line
#define LED    13
//#define SPEED  45

Servo Left;
Servo Right;

ros::NodeHandle nh;

int rightSpeed, leftSpeed;
double relative_angle;
int motorEnabledFlag=0;
unsigned long cutoffMillis;

void messageCb(const std_msgs::Float32& str){
        
        if(str.data == -1){ 
          
                 leftSpeed=110;              //no free-space front Zumo robot, rotate to find another one
                 rightSpeed =110;
                 rotatePort(leftSpeed, rightSpeed,  100);
                 //searchPort(100,100);
        }
        else
        {
            relative_angle=abs(90.0-str.data);
            
            if (relative_angle <= 2.0)         //optional direction in front of Zumo robot
            {
                 leftSpeed=140;
                 rightSpeed =40;
                 forwardPort(leftSpeed, rightSpeed, 100);
            }
            else
            {
                 if (str.data <90.0)
                 {
                     leftSpeed=100+relative_angle;
                     leftSpeed=min(leftSpeed, 155);
                     rightSpeed =80;
                     
                     turnPort(leftSpeed,rightSpeed,  100);
                 }
                 else
                 {
                     leftSpeed=100;
                     rightSpeed =80-relative_angle;
                     rightSpeed=max(rightSpeed, 25);
                     turnPort(leftSpeed,rightSpeed,  100);                    
                 }     
            }     
            
        }
}

ros::Subscriber<std_msgs::Float32> sub("/space1/move_zumo", &messageCb);

void setup(){
  pinMode(LED, OUTPUT);
  Left.attach(9); //use PWM pin 9 to control left motors
  Right.attach(10); // use PWM pin 10 to control right motors
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  nh.spinOnce();
  
  if (motorEnabledFlag) 
  {
       if (millis () >=cutoffMillis)
        {
            Left.write(90);
            Right.write(90);
            motorEnabledFlag=0;
        }
  }
  delay(50);
}




void rotatePort  (int LSpeed, int RSpeed, int T1)
{
    Left.write(LSpeed);
    Right.write(RSpeed);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;
}

void forwardPort (int LSpeed, int RSpeed,  int T1)
{
    Left.write(LSpeed);
    Right.write(RSpeed);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;   
}

void turnPort(int LSpeed, int RSpeed,  int T1)
{
    Left.write(LSpeed);
    Right.write(RSpeed);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;    
}

//void searchPort(int T1, int T2)
//{
  //  digitalWrite(LED,HIGH);
   // delay(T1);
   // digitalWrite(LED,LOW);
   // delay(T2); 
//}


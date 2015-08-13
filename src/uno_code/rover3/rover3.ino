#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <Servo.h>

#define LED    13

Servo Left;
Servo Right;

ros::NodeHandle nh;

int rightSpeed, leftSpeed;
int motorEnabledFlag=0;
unsigned long cutoffMillis;
double dire;

void messageCb(const std_msgs::Float32MultiArray& str){
  
         dire= str.data[1];

        if(dire == -1.0){ 
          
                 leftSpeed=60;              //no free-space front Zumo robot, rotate to find another one
                 rightSpeed =60;
                 rotatePort(leftSpeed, rightSpeed,  50);
        }
        else
        {
           double  relative_angle=abs(90.0-dire);
            
            if (relative_angle <= 20.0)         //optional direction in front of Zumo robot
            {
                 leftSpeed=120;
                 rightSpeed =60;
                 forwardPort(leftSpeed, rightSpeed, 50);
            }
            else
            {
                 if (dire <90.0)
                 {
                     leftSpeed=120;
                     rightSpeed =110;
                     //rightSpeed =max(rightSpeed,40);
                     turnPort(leftSpeed,rightSpeed,  50);
                 }
                 else
                 {
                     
                     leftSpeed=70;
                     //leftSpeed=min(leftSpeed, 140);
                     rightSpeed =60;
       
                     turnPort(leftSpeed,rightSpeed,  50);                    
                 }     
            }     
            
        }
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("/space1/move_zumo", messageCb);
std_msgs::Float32MultiArray test;
ros::Publisher p ("topic", &test);

void setup(){
  pinMode(LED, OUTPUT);
  Left.attach(9); //use PWM pin 9 to control left motors
  Right.attach(10); // use PWM pin 10 to control right motors

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(p);
}

void loop(){
  nh.spinOnce();
  

  p.publish(&test);
  if (motorEnabledFlag)
  {
       if (millis () >=cutoffMillis)
        {
            //Left.write(90);
           // Right.write(90);
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


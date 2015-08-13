//two equation to determine input value

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <Servo.h>

Servo Left;
Servo Right;

ros::NodeHandle nh;

int rightSpeed, leftSpeed;
int motorEnabledFlag=0;
unsigned long cutoffMillis;
//int rotateFlag=0;
int Vr1=140;
int Vr2=120;

void messageCb(const std_msgs::Float32MultiArray& str){
  
        //double dis_infornt=str.data[0];  
        double dire= str.data[1];

        if(dire == -1.0){ 
          
                 leftSpeed=120;              //no free-space front Zumo robot, rotate to find another one
                 rightSpeed =120;
                 rotatePort(leftSpeed, rightSpeed,  100);
        }
        else
        {
           double  relative_angle=abs(90.0-dire);
            
            if (relative_angle <= 2.0)         //optional direction in front of Zumo robot
            {
                 leftSpeed=120;
                 rightSpeed =60;
                 forwardPort(leftSpeed, rightSpeed, 100);
            }
            else
            {
                 if (dire >=0 && dire <=55)
                 {
                   leftSpeed=120;
                   rightSpeed =89.25-0.075*dire;
                   //rightSpeed =max(rightSpeed,60);
                   turnPort(leftSpeed,rightSpeed,  100);                   

                 }
                 else if (dire > 55 && dire < 90.0)
                 {                    
                   leftSpeed=140;
                   rightSpeed=89.7-0.16*dire;
                   turnPort(leftSpeed,rightSpeed,  100);                   
                 }
                 else if (dire >90 && dire <125)
                 {
                   double mirror_angle=180-dire;
                   double ratio=50/(2.9+0.16*mirror_angle);
                   leftSpeed=90+50/ratio;
                   rightSpeed=40;
                 }
                 else
                 {
                   double mirror_angle=180-dire;
                   double ratio=30/(0.75+0.075*mirror_angle);                     
                   leftSpeed=90+30/ratio;
                   //leftSpeed=min(leftSpeed, 140);
                   rightSpeed =60;       
                   turnPort(leftSpeed,rightSpeed,  100);                    
                 }     
            }     
            
        }
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("/space1/move_zumo", messageCb);

void setup(){
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
            //Left.write(90);
            //Right.write(90);
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


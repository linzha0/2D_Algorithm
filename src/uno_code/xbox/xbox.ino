//one equation to determine input value
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <Servo.h>

Servo Left;
Servo Right;

ros::NodeHandle nh;

int rightSpeed, leftSpeed;

void messageCb(const std_msgs::Float32& str){
  
   
        double dire= str.data;

        if(dire == -1.0){ 
                   
          
                 leftSpeed=90;              //no free-space front Zumo robot, rotate to find another one
                 rightSpeed =90;
                 mode(leftSpeed, rightSpeed);
        }
        else
        {
           double  relative_angle=abs(90.0-dire);
            
            if (relative_angle <= 8.0)         //optional direction in front of Zumo robot
            {
                 leftSpeed=115;
                 rightSpeed =65;
                 mode(leftSpeed, rightSpeed);
            }
            else
            {
                 if (dire <90)
                 {
                   leftSpeed=135;
                   rightSpeed =88.3125-0.13125*dire;
                   mode(leftSpeed,rightSpeed);                   

                 }

                 else
                 {
                   double mirror_angle=180-dire;
                   double ratio=45/(1.6875+0.13125*mirror_angle);                     
                   leftSpeed=90+45/ratio;
                   rightSpeed =45;       
                   mode(leftSpeed,rightSpeed);                    
                 }     
            }     
            
        }
}

ros::Subscriber<std_msgs::Float32> sub("move_zumo", messageCb);

void setup(){
  Left.attach(9); //use PWM pin 9 to control left motors
  Right.attach(10); // use PWM pin 10 to control right motors
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  
  nh.spinOnce();
  delay(10);
  
}


void mode(int LSpeed, int RSpeed)
{
    Left.write(LSpeed);
    Right.write(RSpeed);
   
}

#include <ros.h>
#include <std_msgs/Float32.h>

#define DIR_R  7             //Right motor Direction control line
#define DIR_L  8             //Left motor Direction control line
#define PWM_R  9             //Right motor PWM control line
#define PWM_L  10            //Left motor PWM control line
#define LED    13
#define SPEED  45
ros::NodeHandle nh;

int rightSpeed, leftSpeed;
double relative_angle;
int motorEnabledFlag=0;
unsigned long cutoffMillis;

void messageCb(const std_msgs::Float32& str){
        
        //int rightSpeed, leftSpeed;
        
        if(str.data == -1){                    //no free-space front Zumo robot, rotate to find another one
                 rightSpeed =SPEED;
                 leftSpeed  =SPEED;
                 rotatePort(rightSpeed, leftSpeed, 100);
                 searchPort(100,100);
        }
        else
        {
            relative_angle=abs(90.0-str.data);
            
            if (relative_angle <= 2.0)         //optional direction in front of Zumo robot
            {
                 rightSpeed =SPEED;
                 leftSpeed  =SPEED;
                 forwardPort(rightSpeed, leftSpeed, 100);
            }
            else
            {
                 if (str.data <90.0)
                 {
                     rightSpeed =SPEED - relative_angle/90.0*SPEED;
                     leftSpeed  =SPEED + relative_angle/90.0*SPEED;
                     turnPort(rightSpeed, leftSpeed, 100);
                 }
                 else
                 {
                     rightSpeed =SPEED + relative_angle/90.0*SPEED;
                     leftSpeed  =SPEED - relative_angle/90.0*SPEED;
                     turnPort(rightSpeed, leftSpeed, 100);                     
                 }     
            }     
            
        }
}

ros::Subscriber<std_msgs::Float32> sub("/space1/move_zumo", &messageCb);

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(DIR_L, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  nh.spinOnce();
  
  if (motorEnabledFlag) 
  {
       if (millis () >=cutoffMillis)
        {
            analogWrite(PWM_R, 0);
            analogWrite(PWM_L, 0); 
            motorEnabledFlag=0;
        }
  }
  delay(50);
}




void rotatePort  (int rs, int ls, int T1)
{
    digitalWrite(DIR_R, LOW);
    digitalWrite(DIR_L, HIGH);
    analogWrite(PWM_R, rs);
    analogWrite(PWM_L, ls);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;
    //delay(T1);
   // analogWrite(PWM_R, 0);
    //analogWrite(PWM_L, 0);
    //delay(T2);
}

void forwardPort (int rs, int ls, int T1)
{
    digitalWrite(DIR_R, LOW);
    digitalWrite(DIR_L, LOW);
    analogWrite(PWM_R, rs);
    analogWrite(PWM_L, ls);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;   
}

void turnPort(int rs, int ls, int T1)
{
    digitalWrite(DIR_R, LOW);
    digitalWrite(DIR_L, LOW);
    analogWrite(PWM_R, rs);
    analogWrite(PWM_L, ls);
    delay(T1);
    motorEnabledFlag=1;
    cutoffMillis= millis()+T1;      
}

void searchPort(int T1, int T2)
{
    digitalWrite(LED,HIGH);
    delay(T1);
    digitalWrite(LED,LOW);
    delay(T2); 
}


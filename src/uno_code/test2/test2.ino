#include <ros.h>
#include <std_msgs/Int8.h>

int R = 7;            //Right motor Direction control line
int L = 8;            //Left motor Direction control line
int RM = 9;           //Right motor PWM control line
int LM = 10;          //Left motor PWM control line

ros::NodeHandle nh;

void messageCb(const std_msgs::Int8& str){
        if(str.data == 1){
            backProt(75, 750, 1);
        }
        else if(str.data == 2){
            forwardProt(75, 750, 1);
        }
        else if(str.data == 3){
            leftProt(75, 740, 1);    //~60deg
        }
        else if(str.data == 4){
            leftProt(75, 400, 1);    //~30deg
        }
        else if(str.data == 5){
            leftProt(75, 150, 1);    //~10deg
        }
        else if(str.data == 6){
            leftProt(75, 100, 1);     //~5deg
        }
        else if(str.data == 7){
            leftProt(75, 50, 1);      //~1deg
        }
        else if(str.data == 8){
            rightProt(75, 740, 1);
        }
        else if(str.data == 9){
            rightProt(75, 400, 1);
        }
        else if(str.data == 10){
            rightProt(75, 150, 1);
        }
        else if(str.data == 11){
            rightProt(75, 100, 1);
        }
        else if(str.data == 12){
            rightProt(75, 50, 1);
        }
}

ros::Subscriber<std_msgs::Int8> sub("/space1/move_zumo", &messageCb);

void setup(){
  pinMode(13, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(L, OUTPUT);
  pinMode(RM, OUTPUT);
  pinMode(LM, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  nh.spinOnce();
  delay(20);
}













void forwardProt(int P, int T1, int T2){
    digitalWrite(R, LOW);
    digitalWrite(L, LOW);
    analogWrite(RM, P);
    analogWrite(LM, P);
    delay(T1);
    analogWrite(RM, 0);
    analogWrite(LM, 0);
    delay(T2);
}

void backProt(int P, int T1, int T2){
    digitalWrite(R, HIGH);
    digitalWrite(L, HIGH);
    analogWrite(RM, P);
    analogWrite(LM, P);
    delay(T1);
    analogWrite(RM, 0);
    analogWrite(LM, 0);
    delay(T2);
}

void leftProt(int P, int T1, int T2){
    digitalWrite(R, LOW);
    digitalWrite(L, HIGH);
    analogWrite(RM, P);
    analogWrite(LM, P);
    delay(T1);
    analogWrite(RM, 0);
    analogWrite(LM, 0);
    delay(T2);
}

void rightProt(int P, int T1, int T2){
    digitalWrite(R, HIGH);
    digitalWrite(L, LOW);
    analogWrite(RM, P);
    analogWrite(LM, P);
    delay(T1);
    analogWrite(RM, 0);
    analogWrite(LM, 0);
    delay(T2);
}

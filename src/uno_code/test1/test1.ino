/* 
    Zumo Shield Test1 Driving with wasd
*/

int R = 7;            //Right motor Direction control line
int L = 8;            //Left motor Direction control line
int RM = 9;           //Right motor PWM control line
int LM = 10;          //Left motor PWM control line
char ch;

void setup(){
    pinMode(R, OUTPUT);
    pinMode(L, OUTPUT);
    pinMode(RM, OUTPUT);
    pinMode(LM, OUTPUT);
    Serial.begin(9600);
}

void loop(){
    if(Serial.available()){
        ch = Serial.read();
        Serial.println(ch);
        if(ch == 'w'){
            forwardProt(75, 1000, 500);
        }
        else if(ch  == 'a'){
            leftProt(75, 1000, 500);
        }
        else if(ch  == 's'){
            backProt(75, 1000, 500);
        }
        else if(ch  == 'd'){
            rightProt(75, 1000, 500);
        }
    }
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

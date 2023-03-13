/*
請上傳此程式碼至Arduino Uno
*/
#include<SoftwareSerial.h>
SoftwareSerial uno(3,2);

const int trigPin = 5;
const int echoPin = 6;
int in1 = 9;
int in2 = 10;
int in3 = 11;
int in4 = 12;
long duration;
int distance;
bool flag = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);

  Serial.begin(9600);
  Serial.println("start");
  uno.begin(4800);
}

//朝正向轉
void doleON() {
  digitalWrite(9, HIGH);
  digitalWrite(11, HIGH);
}
//停止朝正向轉
void doleOFF() {
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);
}
//朝反向轉
void goreON() {
  digitalWrite(10, HIGH);
  digitalWrite(12, HIGH);
}
//停止朝反向轉
void goreOFF() {
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
}

//更新distance
void merenje_udaljenosti(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

//電梯移動到一樓(distance==14)
void moveOne(){
  if(distance < 15){
      delay(1000);
      goreOFF();
      flag = true;
      while(flag){
        doleON();
        merenje_udaljenosti();
        if(distance==15){
          doleOFF();
          flag=false;
        }
      }
    }
    else{
      goreOFF();
      doleOFF();
    }
}

//電梯移動到二樓(distance==9)
void moveTwo(){
    if(distance < 12){
      delay(1000);
      goreOFF();
      flag = true;
      while(flag){
        merenje_udaljenosti();
        delay(500);
        doleON();
        if(distance==12){
          doleOFF();
          flag=false;
        }
      }
    }
    else if(distance>12){
      delay(1000);
      doleOFF();
      flag = true;
      while(flag){
        merenje_udaljenosti();
        delay(500);
        goreON();
        if(distance==12){
          goreOFF();
          flag=false;
        }
      }
    }
    else{
      goreOFF();
      doleOFF();
    }
}

//電梯移動到三樓(distance==4)
void moveThree(){
    if(distance < 8){
      delay(1000);
      goreOFF();
      doleOFF();
    }
    else if(distance > 8){
      delay(1000);
      doleOFF();
      flag = true;
      while(flag){
        goreON();
        merenje_udaljenosti();
        if(distance==8){
          goreOFF();
          flag=false;
        }
      }
    }
    else{
      goreOFF();
      doleOFF();
    }
}

void loop() {
  //merenje_udaljenosti();

  while(uno.available()>0){
    int val = uno.parseInt();
    if(uno.read() == '\n'){
      Serial.println(val);

      if(val == 1){
        Serial.println("successfully received 1");
        merenje_udaljenosti();
        moveOne();
      }
      else if(val == 2){
        Serial.println("successfully received 2");
        merenje_udaljenosti();
        moveTwo();
      }
      else if(val == 3){
        Serial.println("successfully received 3");
        merenje_udaljenosti();
        moveThree();
      }
      else{
        Serial.println("error");
      }

    }
  }
  delay(30);

}
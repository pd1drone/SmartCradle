#include <Servo.h>

const int PushBtn = 6;
Servo myservo;  
int pos = 0;  
int MotorSpeed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PushBtn, INPUT_PULLUP);
  myservo.attach(5);
  myservo.write(pos);
}

void loop() {
  // put your main code here, to run repeatedly:
  int PushbtnValue = digitalRead(PushBtn);
  if (PushbtnValue == 0) {
    while (true) {
      PushbtnValue = digitalRead(PushBtn);
      if (PushbtnValue == 1) {
        if (MotorSpeed == 3){
          MotorSpeed = 0;
        }else{
          MotorSpeed = MotorSpeed + 1;
        }
        break;
      }
    }
  }

  if (MotorSpeed == 0){
    myservo.write(0);
  }else if (MotorSpeed == 1){
    for(pos = 0; pos <=180; pos +=7){
      myservo.write(pos);
      delay(50);
    }
    for(pos = 180; pos>=0; pos -=7){
      myservo.write(pos);
      delay(50);
    }
  }else if (MotorSpeed ==2){
    for(pos = 0; pos <=180; pos +=7){
      myservo.write(pos);
      delay(25);
    }
    for(pos = 180; pos>=0; pos -=7){
      myservo.write(pos);
      delay(25);
    }
  }else if (MotorSpeed ==3){
    for(pos = 0; pos <=180; pos +=7){
      myservo.write(pos);
    }
    for(pos = 180; pos>=0; pos -=7){
      myservo.write(pos);
    }
  }

}



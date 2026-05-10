#include <Servo.h>

int frontrightmotorpin1 = 2;
int frontrightmotorpin2 = 3;

int frontleftmotorpin1 = 4;
int frontleftmotorpin2 = 5;

int backrightmotorpin1 = 7;
int backrightmotorpin2 = 8;

int backleftmotorpin1 = 12;
int backleftmotorpin2 = 13;

int speedpin = 6;

int trigger1 = 9;
int listener1 = A1;

int trigger2 = 10;
int listener2 = A0;

Servo Scooper;
int servopin = 11;
bool active = false;
String command = "";

int spd = 150;
int tim = 1000;

bool power = true;

long duration1 = 0;
long duration2 = 0;
int distance1 = 0;
int distance2 = 0;

double longitude = 0;
double latitude = 0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A2));

  pinMode(frontrightmotorpin1, OUTPUT);
  pinMode(frontrightmotorpin2, OUTPUT);
  pinMode(frontleftmotorpin1, OUTPUT);
  pinMode(frontleftmotorpin2, OUTPUT);
  pinMode(backrightmotorpin1, OUTPUT);
  pinMode(backrightmotorpin2, OUTPUT);
  pinMode(backleftmotorpin1, OUTPUT);
  pinMode(backleftmotorpin2, OUTPUT);

  pinMode(speedpin, OUTPUT);

  pinMode(trigger1, OUTPUT);
  pinMode(listener1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(listener2, INPUT);

  Scooper.attach(servopin);
  Scooper.write(0);
}

void loop() {
  if(power){
    getdistance();
    move();
    gps();
  }

  if(Serial.available() > 0){
    command = Serial.readStringUntil('\n');
    command.trim();

    if(command == "SCOOP" && power){
      toggleScoop();
    }
    else if(command == "STATUS" && power){
      if(active){
        Serial.println("ACTIVE");
      }
      else{
        Serial.println("INACTIVE");
      }
    }
    else if(command == "GPS" && power){
      Serial.print(latitude);
      Serial.print(" ");
      Serial.println(longitude);
    }
    else if(command == "ON"){
      power = true;
    }
    else if(command == "OFF"){
      stop();
      power = false;
    }
  }
}

void setspeed(int speed){
  analogWrite(speedpin, speed);
}

void stop(){
  digitalWrite(frontrightmotorpin1, LOW);
  digitalWrite(frontrightmotorpin2, LOW);

  digitalWrite(frontleftmotorpin1, LOW);
  digitalWrite(frontleftmotorpin2, LOW);

  digitalWrite(backrightmotorpin1, LOW);
  digitalWrite(backrightmotorpin2, LOW);

  digitalWrite(backleftmotorpin1, LOW);
  digitalWrite(backleftmotorpin2, LOW);

  setspeed(0);
}

void forward(int speed){
  setspeed(speed);

  digitalWrite(frontrightmotorpin1, LOW);
  digitalWrite(frontrightmotorpin2, HIGH);

  digitalWrite(frontleftmotorpin1, HIGH);
  digitalWrite(frontleftmotorpin2, LOW);

  digitalWrite(backrightmotorpin1, HIGH);
  digitalWrite(backrightmotorpin2, LOW);

  digitalWrite(backleftmotorpin1, LOW);
  digitalWrite(backleftmotorpin2, HIGH);
}

void turnright(int speed, int time){
  stop();
  setspeed(speed);

  digitalWrite(frontrightmotorpin1, HIGH);
  digitalWrite(frontrightmotorpin2, LOW);

  digitalWrite(frontleftmotorpin1, HIGH);
  digitalWrite(frontleftmotorpin2, LOW);

  digitalWrite(backrightmotorpin1, LOW);
  digitalWrite(backrightmotorpin2, HIGH);

  digitalWrite(backleftmotorpin1, LOW);
  digitalWrite(backleftmotorpin2, HIGH);

  delay(time);
  stop();
}

void turnleft(int speed, int time){
  stop();
  setspeed(speed);

  digitalWrite(frontrightmotorpin1, LOW);
  digitalWrite(frontrightmotorpin2, HIGH);

  digitalWrite(frontleftmotorpin1, LOW);
  digitalWrite(frontleftmotorpin2, HIGH);

  digitalWrite(backrightmotorpin1, HIGH);
  digitalWrite(backrightmotorpin2, LOW);

  digitalWrite(backleftmotorpin1, HIGH);
  digitalWrite(backleftmotorpin2, LOW);

  delay(time);
  stop();
}

void toggleScoop(){
  if(active){
    Scooper.write(0);
    active = false;
  }
  else{
    Scooper.write(90);
    active = true;
  }
}

void getdistance(){
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);

  duration1 = pulseIn(listener1, HIGH, 20000);
  delay(30);

  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);

  duration2 = pulseIn(listener2, HIGH, 20000);
  distance1 = duration1 * 0.034 / 2;
  distance2 = duration2 * 0.034 / 2;
}

void move(){
  if(distance1 == 0){
    distance1 = 999;
  }
  if(distance2 == 0){
    distance2 = 999;
  }
  if(distance1 <= 40 || distance2 <= 40){
    delay(20);
    getdistance();
    if(distance1 <= 40 || distance2 <= 40){
      stop();
      turnright(spd, 400);
      delay(100);
      forward(spd);
    }
  }
  else{
    forward(spd);
  }
}

void gps(){
  latitude = random(42240, 42360) / 1000.0;
  longitude = random(-83120, -82880) / 1000.0;
}
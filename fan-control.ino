/*
 * 
 * FAN CONTROL AGAINST MOLD
 * 
 */

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;


int fan_pin = 5;
int sensor = A0;
int BAD_LEVEL = 40;
int SPEED = 0;

unsigned long T = 60UL*1000UL*60UL * 1UL; // 1 HOURS

byte fan_status = 0;
void setup() {
  pinMode(fan_pin, OUTPUT);
  analogWrite(fan_pin,0);
  pinMode(sensor, INPUT);

  Serial.begin(9600);
  Serial.println(".");
  Serial.println("START FAN CONTROL --------------------");

}

unsigned long timer = 0;

void loop() {
  
    DHT.read(sensor);
  
    if(fan_status==0 && DHT.humidity < BAD_LEVEL  && millis() > timer ) {
        timer = millis() + T;
        fan_status=1;
        SPEED = 153;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==0 && DHT.humidity >= BAD_LEVEL) {
        fan_status= 2;
        SPEED = 255;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==2 && DHT.humidity < BAD_LEVEL) {
        fan_status= 0;
        timer = millis() + T;
        SPEED = 0;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==1 && DHT.humidity >= BAD_LEVEL) {
        fan_status= 2;
        SPEED = 255;
        analogWrite(fan_pin, SPEED);
        timer = millis() + T;
    }

    if(fan_status==1 && DHT.humidity < BAD_LEVEL && millis() > timer) {
        fan_status= 0;
        timer = millis() + T;
        SPEED = 0;
        analogWrite(fan_pin, SPEED); 
    }

    if(fan_status==2 && DHT.humidity >= BAD_LEVEL) {
      timer = millis() + T;
    }


    Serial.print("HUM ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("T ");
    Serial.print(DHT.temperature); 
    Serial.print("°C  ");
    Serial.print("FAN ");
    Serial.print(fan_status);
    Serial.print("  SPEED ");
    Serial.print(SPEED);
    unsigned long t = (timer - millis()) / 1000UL;
    Serial.println("  TIMER " + (String)t + "sec");

    delay(5000);
  
  
}

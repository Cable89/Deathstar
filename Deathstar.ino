/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define IKEA_PIN 15
#define IKEA_MAX 1666
//      IKEA_MID 370
#define IKEA_MIN 100

#define DELAY 20
#define DOWNSAMPLE_DEF 10

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int pwm_value = 0;
int norm_pwm_value = 0;
int static_value = 0;
int angle = 0;
int pwm_buffer[12] = {IKEA_MIN,IKEA_MIN,IKEA_MIN,IKEA_MIN,IKEA_MIN,IKEA_MIN};
int i = 0;
int k = 0;
int avg = IKEA_MIN;
int delay_t = DELAY;
int downsample = DOWNSAMPLE_DEF;


void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(IKEA_PIN, INPUT);
  //pinMode(8, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  pwm_value = pulseIn(IKEA_PIN, HIGH);
  delay_t = DELAY;
  downsample = DOWNSAMPLE_DEF;
  if(pwm_value == 0){
    delay_t = 0;
    downsample = 0;
    static_value = digitalRead(IKEA_PIN);
    if(static_value){
      pwm_value = IKEA_MAX;
    }else{
      pwm_value = IKEA_MIN;
    }
  }else if(pwm_value > IKEA_MAX){
    pwm_value = IKEA_MAX;
  }else if(pwm_value < IKEA_MIN){
    pwm_value = IKEA_MIN;
  }

  Serial.println("pwm_value");
  Serial.println(pwm_value);
  Serial.println("avg");
  Serial.println(avg);
  
  if(((pwm_value*0.8) > avg) || ((pwm_value*1.2) < avg)){
    digitalWrite(8, LOW);
    norm_pwm_value = sqrt(pwm_value - IKEA_MIN);
    angle = norm_pwm_value * 4.6;
    angle = 180 - angle;
    Serial.println("angle");
    Serial.println(angle);
    myservo.write(angle);
  }else{
    Serial.println("Servo off");
    
    digitalWrite(8, HIGH);
  }

  avg = 0;
  for(int j=0; j<6; j++){
    avg += pwm_buffer[j];
  }
  avg = avg/6;
  
  i++;
  if(i>downsample){
    i = 0;
    k++;
    pwm_buffer[k] = pwm_value;
  }
  if(k>6){
    k = 0;
  }
  delay(delay_t);
}


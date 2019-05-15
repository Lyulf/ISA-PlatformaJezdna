#include "ISAMobile.h"
#include <math.h>

#define MAX_ANGLE_DIFFERENCE 5
#define NUMBER_OF_INITIAL_SAMPLES 5
#define FORWARD_POWER 120
#define TURNING_POWER 150
#define OBSTACLE_DISTANCE 30

QMC5883 qmc;

double right_angles[4];
int driving_mode = 0;
int front_obstruction_filter = 0;
double distance_measured[20];


char buffer[64];
int d[5] = {};
int sum = 0;
int id = 0;

void loop(void);
void setup(void);
double angleDifference(double current, double target);
void setPowerLevel(EngineSelector side, int level);
int measureSoundSpeed(int trigger_pin, int echo_pin);
double getCurrentAngle();
int getFrontDistance();
void correctTargetAngle(double& angle);

void setup(void)
{
  // Czujniki ultradźwiekowe
  for (int i = (int)UltraSoundSensor::__first; i <= (int)UltraSoundSensor::__last; i++)
  {
    pinMode(ultrasound_trigger_pin[i], OUTPUT);
    pinMode(ultrasound_echo_pin[i], INPUT);

    digitalWrite(ultrasound_trigger_pin[i], 0);
  }

  // Silniki
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  //pinMode(MODE, OUTPUT); -- podłaczone na krótko ze stanem wysokim
  //digitalWrite(MODE, true);  -- podłaczone na krótko ze stanem wysokim

  setPowerLevel(EngineSelector::Left, 0);
  setPowerLevel(EngineSelector::Right, 0);

  // Wejścia enkoderowe
  pinMode(ENCODER_LEFT, INPUT);
  pinMode(ENCODER_RIGHT, INPUT);

  Serial.begin(9600);
  Serial.print("Hello Father");

  Wire.begin();
  qmc.init();

  Serial1.begin(9600); // HC06
  Serial1.print("Hello Father\n");

  delay(2000);

  //finds initial angle based on average of measurements

  Serial1.print("I have awakened\n");

  //calculating intitial angle based on average of multiple samples
  double initial_angle = getCurrentAngle();

  sprintf(buffer, "\n initial_angle = %f", initial_angle);
  Serial1.print(buffer);

  //find 4 right angles

  //hardcoded values
  right_angles[0] = 70;
  right_angles[1] = 170;
  right_angles[2] = -100;
  right_angles[3] = -22;
  //right_angles[0] = initial_angle;
  for (int i = 0; i < 4; i++) {
    //right_angles[i] = initial_angle + 90 * i;
    //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
    
    sprintf(buffer, "\n right_angle[%d] = %f", i, right_angles[i]);
    Serial1.print(buffer);
  }
}


void loop(void) {

  int dist = getFrontDistance();
  
  static double current_angle;
  static double target_angle;

  //test
  /*
  driving_mode = 2;
  sprintf(buffer, "\n%f", getCurrentAngle());
  Serial1.print(buffer);
  delay(1000);
  */
  
  if (driving_mode == 0) {
    //tryb jazdy do przodu
    setPowerLevel(EngineSelector::Right, FORWARD_POWER);
    setPowerLevel(EngineSelector::Left, FORWARD_POWER);
    if (dist < OBSTACLE_DISTANCE) {
      //filtr
      front_obstruction_filter++;
    }
    else {
      front_obstruction_filter = 0;
    }
    if (front_obstruction_filter > 3) {
      setPowerLevel(EngineSelector::Right, 0);
      setPowerLevel(EngineSelector::Left, 0);

      Serial1.print("\n I see an obstacle before me");
      delay(1000);
      
      
      current_angle = getCurrentAngle();
      
      //target angle może być zwracany przez funkcje na zasadzie 'seek route'
      target_angle = current_angle + 90;
      target_angle -= target_angle > 180 ? 360 : 0;
      sprintf(buffer, "\n trg before: %f", target_angle);
      Serial1.print(buffer);
      correctTargetAngle(target_angle);
      sprintf(buffer, "     trg after: %f", target_angle);
      Serial1.print(buffer);
      
      front_obstruction_filter = 0;
      driving_mode = 1;
    }
  }
  else if(driving_mode == 1){
    //tryb obracania
    setPowerLevel(EngineSelector::Right, -TURNING_POWER);
    setPowerLevel(EngineSelector::Left, TURNING_POWER);

    current_angle = getCurrentAngle();
    
    if(fabs(angleDifference(current_angle, target_angle)) < MAX_ANGLE_DIFFERENCE){
      
      setPowerLevel(EngineSelector::Right, 0);
      setPowerLevel(EngineSelector::Left, 0);
      driving_mode = 0;

      sprintf(buffer, "\n actual: %f", current_angle);
      Serial1.print(buffer);

      sprintf(buffer, "     expected: %f", target_angle);
      Serial1.print(buffer);
  
      Serial1.print("\n Turning protocol concluded, resuming journey");
  
      delay(1000);
    }
    
  }
  
}

void correctTargetAngle(double& angle) {
  double tmp = 0;
  for(int i = 0; i < 4; i++) {
    tmp = angleDifference(angle, right_angles[i]);
    if(fabs(tmp) <= 45) {
      angle = right_angles[i];
      return;
    }
  }
}

int getFrontDistance() {
    //zbieranie dystansu z przodu
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[(int)UltraSoundSensor::Front],
               ultrasound_echo_pin[(int)UltraSoundSensor::Front]);

  // średnia krocząca z pomiarów dystansu z przodu
  sum -= d[id];
  sum += d[id] = dist;
  id = (id + 1) % 5;
  dist = sum / 5;

  return dist;
}

/* Funkcja zwracająca kąt który samochód musi się obrócić na podstawie
 jeśli difference(diff*/
double angleDifference(double current, double target) {
  double diff = target - current;
  if (diff > 180) {
    diff = diff - 360;
  }
  else if (diff < -180) {
    diff = diff + 360;
  }
  return diff;
}

double getCurrentAngle() {
  double avg_angle = 0;
  for (int i = 0; i < NUMBER_OF_INITIAL_SAMPLES; i++) {
    qmc.measure();
    int16_t x = qmc.getX();
    int16_t y = qmc.getY();
    avg_angle += (atan2(x, y) * 180 / PI);
    //delay(10);
  }

 return avg_angle / NUMBER_OF_INITIAL_SAMPLES;
}

void setPowerLevel(EngineSelector side, int level)
{
  level = constrain(level, -255, 255);

  if (side == EngineSelector::Left) {
    if (level > 0) {
      // do przodu
      digitalWrite(LEFT_IN1, false);
      digitalWrite(LEFT_IN2, true);
      analogWrite(LEFT_PWM, level);
    } else if (level < 0) {
      // do tyłu
      digitalWrite(LEFT_IN1, true);
      digitalWrite(LEFT_IN2, false);
      analogWrite(LEFT_PWM, -level);
    } else {
      // stop (soft)
      digitalWrite(LEFT_IN1, true);
      digitalWrite(LEFT_IN2, true);
      analogWrite(LEFT_PWM, 0);
    }
  }
  
  if (side == EngineSelector::Right) {
    if (level > 0) {
      // do przodu
      digitalWrite(RIGHT_IN1, true);
      digitalWrite(RIGHT_IN2, false);
      analogWrite(RIGHT_PWM, level);
    } else if (level < 0) {
      // do tyłu
      digitalWrite(RIGHT_IN1, false);
      digitalWrite(RIGHT_IN2, true);
      analogWrite(RIGHT_PWM, -level);
    } else {
      // stop (soft)
      digitalWrite(RIGHT_IN1, true);
      digitalWrite(RIGHT_IN2, true);
      analogWrite(RIGHT_PWM, 0);
    }
  } 
}


int measureSoundSpeed(int trigger_pin, int echo_pin)
{
  digitalWrite(trigger_pin, false);
  delayMicroseconds(2);

  digitalWrite(trigger_pin, true);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, false);

  // zmierz czas przelotu fali dźwiękowej
  int duration = pulseIn(echo_pin, true, 50 * 1000);


  // przelicz czas na odległość (1/2 Vsound(t=20st.C))
  int distance = (int)((float)duration * 0.03438f * 0.5f);
  return distance;
}

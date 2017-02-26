
#include <Servo.h>


//Config
const int SERVO_PIN_VERT = 9;
const int SERVO_PIN_HORZ = 6;
const int LASER_PIN = 4;
const int SWITCH_PIN = 2;

const int ANGLE_MAX_VERT = 80;
const int ANGLE_MIN_VERT = 10;
const int ANGLE_MAX_HORZ = 160;
const int ANGLE_MIN_HORZ = 20;
const int START_ANGLE_VERT = 45;
const int START_ANGLE_HORZ = 90;

const int JITTER_LEVEL_MIN = 1;
const int JITTER_LEVEL_MAX = 10;

const int LOOP_DELAY = 10;
const int JITTER_COUNT_MAX = 5; //How many loops before the jitter direciton is reset
const float JITTER_SENSITIVITY_MAX = .3 * LOOP_DELAY; //How intense the jitter is (how far it can wander in a single loop)
const int JITTER_PAUSE_MAX = 10; //How long of a pause is taken between jitters. Random number between 0 and this is taken

Servo servoVert;
Servo servoHorz;
float servoAngleVert = START_ANGLE_VERT; //in degrees
float servoAngleHorz = START_ANGLE_HORZ; //in degrees

//Operational variables
bool laserOn = false;
int jitterCount = 0; //Timer used for jitter direction
int jitterPause = 0; //asssigned random # from 0 to jitterPauseMax
int jitterLevel = 5; //1-10
float jitterSensitivity = 0; //How intense jitter is (based on jitterLevel)
float currJitterAngleVert = 0; //angle used for jitter direction
float currJitterAngleHorz = 0; //angle used for jitter direction
long lastLoopMillis = 0;

void setup ()
{
  Serial.begin(9600);
  servoVert.attach(SERVO_PIN_VERT);
  servoHorz.attach(SERVO_PIN_HORZ);
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  calculateJitterSensitivity();
  recenter();
}

void calculateJitterSensitivity()
{
  jitterSensitivity = JITTER_SENSITIVITY_MAX / (float)JITTER_LEVEL_MAX * (float)jitterLevel;
}

void recenter ()
{
  servoVert.write(START_ANGLE_VERT);
  servoHorz.write(START_ANGLE_HORZ);
}

void loop()
{
//  if (irrecv.decode(&ir_results))
//  {
//    ButtonPress button = translateIR();
//    switch (button)
//    {
//      case POWER: laserOn = !laserOn;
//        if (laserOn)
//        {
//          digitalWrite(LASER_PIN, HIGH);
//        }
//        else
//        {
//          recenter();
//          digitalWrite(LASER_PIN, LOW);
//        }
//        break;
//      case PLUS:
//        jitterLevel++;
//        if (jitterLevel > JITTER_LEVEL_MAX)
//        {
//          jitterLevel = JITTER_LEVEL_MAX;
//        }
//        calculateJitterSensitivity();
//        break;
//      case MINUS:
//        jitterLevel--;
//        if (jitterLevel < JITTER_LEVEL_MIN)
//        {
//          jitterLevel = JITTER_LEVEL_MIN;
//        }
//        calculateJitterSensitivity();
//        break;
//    }
//    irrecv.resume(); // receive the next value
//  }

  boolean switchOn = digitalRead(SWITCH_PIN) == LOW;

  if (switchOn != laserOn)
  {
    //switch has changed
    laserOn = switchOn;
    recenter();
    if (laserOn)
    {
      digitalWrite(LASER_PIN, HIGH);
    }
    else
    {
      digitalWrite(LASER_PIN, LOW);
    }
  }

  if (!laserOn || millis() - lastLoopMillis < LOOP_DELAY)
  {
    return;
  }

  //Calculate new jitter if necessary
  if (jitterCount == 0)
  {
    jitterPause = random(JITTER_PAUSE_MAX);
    jitterCount = JITTER_COUNT_MAX + jitterPause;
    currJitterAngleVert = random(-jitterSensitivity * 1000, jitterSensitivity * 1000) / 1000.0;
    currJitterAngleHorz = random(-jitterSensitivity * 1000, jitterSensitivity * 1000) / 1000.0;
  }
  else if (jitterCount <= JITTER_COUNT_MAX)
  {
    //Pause will happen while counting down from JITTER_COUNT_MAX + jitterPause
    servoAngleVert += currJitterAngleVert;
    servoAngleHorz += currJitterAngleHorz;
  }

  jitterCount--;

  if (servoAngleVert > ANGLE_MAX_VERT)
  {
    servoAngleVert = ANGLE_MAX_VERT;
  }
  if (servoAngleVert < ANGLE_MIN_VERT)
  {
    servoAngleVert = ANGLE_MIN_VERT;
  }
  if (servoAngleHorz > ANGLE_MAX_HORZ)
  {
    servoAngleHorz = ANGLE_MAX_HORZ;
  }
  if (servoAngleHorz < ANGLE_MIN_HORZ)
  {
    servoAngleHorz = ANGLE_MIN_HORZ;
  }

  servoVert.write(servoAngleVert);
  servoHorz.write(servoAngleHorz);
  lastLoopMillis = millis();
  delay(LOOP_DELAY);
}



#include <Servo.h>
#include "IRremote.h"

enum ButtonPress
{
  POWER,
  PLUS,
  MINUS
};

//Config
const int SERVO_PIN_VERT = 9;
const int SERVO_PIN_HORZ = 6;
const int LASER_PIN = 4;
const int IR_PIN = 11;

const int ANGLE_MAX_VERT = 80;
const int ANGLE_MIN_VERT = 10;
const int ANGLE_MAX_HORZ = 160;
const int ANGLE_MIN_HORZ = 20;
const int START_ANGLE_VERT = 45;
const int START_ANGLE_HORZ = 90;

const int JITTER_LEVEL_MIN = 1;
const int JITTER_LEVEL_MAX = 10;

const int LOOP_DELAY = 5;
const int JITTER_COUNT_MAX = 6; //How many loops before the jitter direciton is reset
const float JITTER_SENSITIVITY_MAX = 2.7 * LOOP_DELAY; //How intense the jitter is (how far it can wander in a single loop)
const int JITTER_PAUSE_MAX = 6; //How long of a pause is taken between jitters. Random number between 0 and this is taken

IRrecv irrecv(IR_PIN);
decode_results ir_results;

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
  irrecv.enableIRIn();
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
  if (irrecv.decode(&ir_results))
  {
    ButtonPress button = translateIR();
    switch (button)
    {
      case POWER: laserOn = !laserOn;
        if (laserOn)
        {
          digitalWrite(LASER_PIN, HIGH);
        }
        else
        {
          recenter();
          digitalWrite(LASER_PIN, LOW);
        }
        break;
      case PLUS:
        jitterLevel++;
        if (jitterLevel > JITTER_LEVEL_MAX)
        {
          jitterLevel = JITTER_LEVEL_MAX;
        }
        calculateJitterSensitivity();
        break;
      case MINUS:
        jitterLevel--;
        if (jitterLevel < JITTER_LEVEL_MIN)
        {
          jitterLevel = JITTER_LEVEL_MIN;
        }
        calculateJitterSensitivity();
        break;
    }
    irrecv.resume(); // receive the next value
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

  Serial.println("Jitter Level");
  Serial.println(jitterLevel, DEC);
  Serial.println("Jitter Sensitivity");
  Serial.println(jitterSensitivity, DEC);
//  Serial.println("Servo Angle Vert");
//  Serial.println(servoAngleVert, DEC);
//  Serial.println("Servo Angle Horz");
//  Serial.println(servoAngleHorz, DEC);


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

ButtonPress translateIR()
{

  switch (ir_results.value)
  {

    case 0xE318261B:
    case 0xFFA25D:
      Serial.println(" CH-            ");
      return POWER;

    case 0xFF629D:
      Serial.println(" CH             ");
      break;

    case 0xFFE21D:
      Serial.println(" CH+            ");
      break;

    case 0xFF22DD:
      Serial.println(" PREV           ");
      break;

    case 0xFF02FD:
      Serial.println(" NEXT           ");
      break;

    case 0xFFC23D:
      Serial.println(" PLAY/PAUSE     ");
      break;
    case 0xF076C13B:
    case 0xFFE01F:
      Serial.println(" VOL-           ");
      return MINUS;
    case 0xA3C8EDDB:
    case 0xFFA857:
      Serial.println(" VOL+           ");
      return PLUS;

    case 0xFF906F:
      Serial.println(" EQ             ");
      break;

    case 0xFF6897:
      Serial.println(" 0              ");
      break;

    case 0xFF9867:
      Serial.println(" 100+           ");
      break;

    case 0xFFB04F:
      Serial.println(" 200+           ");
      break;

    case 0xFF30CF:
      Serial.println(" 1              ");
      break;

    case 0xFF18E7:
      Serial.println(" 2              ");
      break;

    case 0xFF7A85:
      Serial.println(" 3              ");
      break;

    case 0xFF10EF:
      Serial.println(" 4              ");
      break;

    case 0xFF38C7:
      Serial.println(" 5              ");
      break;

    case 0xFF5AA5:
      Serial.println(" 6              ");
      break;

    case 0xFF42BD:
      Serial.println(" 7              ");
      break;

    case 0xFF4AB5:
      Serial.println(" 8              ");
      break;

    case 0xFF52AD:
      Serial.println(" 9              ");
      break;

    default:
      Serial.println(" other button   ");

  }
}


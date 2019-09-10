// ---------------------------------------------------------------------------
// Libreria para el uso del robot Escornabot en el entorno visual mBlock
// de MakeBlock mediante la creacion de extensiones
//
// AUTOR/LICENCIA:
// Creado por Angel Villanueva - @avilmaru
// Copyright 2017 Licencia: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
//
// HISTORICO:
// 12/05/2018 v1.0 - Release inicial.
// 26/08/2018 v1.1 - Incorporación de funciones para la utilización de sensor de ultrasonidos,
//                   incorporacion parámetro para cambiar el sentido de la marcha,
//					 funciones de avance y retroceso acordes con el sentido de marcha original
// 10/09/2019 v1.2 - Se añaden las funciones turnLedOnOff(String ledPin,uint8_t state)
//					 y blinkingLed(int16_t num,String ledPin)
// ---------------------------------------------------------------------------

#include <Arduino.h>
#include "EscornabotExtension.h"


EscornabotExtension::EscornabotExtension() {

	// Stepper Engine
	#ifdef ENGINE_TYPE_STEPPERS
	pinMode(STEPPERS_MOTOR_LEFT_IN1, OUTPUT);
	pinMode(STEPPERS_MOTOR_LEFT_IN2, OUTPUT);
	pinMode(STEPPERS_MOTOR_LEFT_IN3, OUTPUT);
	pinMode(STEPPERS_MOTOR_LEFT_IN2, OUTPUT);
	pinMode(STEPPERS_MOTOR_RIGHT_IN1, OUTPUT);
	pinMode(STEPPERS_MOTOR_RIGHT_IN2, OUTPUT);
	pinMode(STEPPERS_MOTOR_RIGHT_IN3, OUTPUT);
	pinMode(STEPPERS_MOTOR_RIGHT_IN4, OUTPUT);
	#endif

	// on-board LED Pin
	#ifdef SIMPLE_LED_PIN
	pinMode(SIMPLE_LED_PIN, OUTPUT);
	#endif

	// KeyPad LEDs
	#ifdef KEYPAD_LED_PIN_UP
	pinMode(KEYPAD_LED_PIN_UP, OUTPUT);
	#endif

	#ifdef KEYPAD_LED_PIN_RIGHT
	pinMode(KEYPAD_LED_PIN_RIGHT, OUTPUT);
	#endif

	#ifdef KEYPAD_LED_PIN_DOWN
	pinMode(KEYPAD_LED_PIN_DOWN, OUTPUT);
	#endif

	#ifdef KEYPAD_LED_PIN_LEFT
	pinMode(KEYPAD_LED_PIN_LEFT, OUTPUT);
	#endif

	#ifdef KEYPAD_LED_PIN_GO
	pinMode(KEYPAD_LED_PIN_GO, OUTPUT);
	#endif

	// Buzzer
	#ifdef BUZZER_PIN
	pinMode(BUZZER_PIN, OUTPUT);
	#endif

	// Button Set Analog
	#ifdef BUTTONS_ANALOG
	pinMode(BS_ANALOG_PIN, BS_ANALOG_WIRES == 2 ? INPUT_PULLUP : INPUT);
	#endif

	// Ultrasonic sensor
	#if ULTRASONIC_SENSOR
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
	#endif


}

EscornabotExtension::~EscornabotExtension() {
}

//////////////////////////////////////////////////////////////////////
/*
 * STEPPERS ENGINE FUNCTIONS
 */
//////////////////////////////////////////////////////////////////////

#ifdef ENGINE_TYPE_STEPPERS

void EscornabotExtension::turnRight(int16_t degrees, float speedFactor)
{

	if(degrees < 0) degrees = 0;
	if(speedFactor <= 0) speedFactor = 1;

	int16_t _speed = (1000000 / (STEPPERS_STEPS_PER_SECOND * speedFactor));

	int32_t steps = STEPPERS_TURN_STEPS;
	steps *= degrees;
	steps /= 90;

	uint8_t stepCounter = 0;

	for (int32_t i = 0; i < steps; i++)
	{
		if (stepCounter >= numSteps) stepCounter = 0;
		motorStepLeft(step_pattern[stepCounter]);
		motorStepRight(step_pattern[stepCounter]);
		delayMicroseconds(_speed);
		stepCounter++;
	}

	motorStepLeft(0);
	motorStepRight(0);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::turnLeft(int16_t degrees, float speedFactor)
{

	if(degrees < 0) degrees = 0;
	if(speedFactor <= 0) speedFactor = 1;

	int16_t _speed = (1000000 / (STEPPERS_STEPS_PER_SECOND * speedFactor));

	int32_t steps = STEPPERS_TURN_STEPS;
	steps *= degrees;
	steps /= 90;

	int8_t stepCounter = numSteps - 1;

	for (int32_t i = 0; i < steps; i++)
	{
		if (stepCounter < 0) stepCounter = numSteps - 1;
		motorStepLeft(step_pattern[stepCounter]);
		motorStepRight(step_pattern[stepCounter]);
		delayMicroseconds(_speed);
		stepCounter--;

	}

	motorStepLeft(0);
	motorStepRight(0);

}

//////////////////////////////////////////////////////////////////////

#if CHANGE_DIRECTION

void EscornabotExtension::moveForward(int16_t units, float speedFactor)
{
	_moveBackward(units,speedFactor);
}

void EscornabotExtension::moveBackward(int16_t units, float speedFactor)
{
	_moveForward(units,speedFactor);
}

#else

void EscornabotExtension::moveForward(int16_t units, float speedFactor)
{
	_moveForward(units,speedFactor);
}

void EscornabotExtension::moveBackward(int16_t units, float speedFactor)
{
	_moveBackward(units,speedFactor);
}

#endif

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::_moveForward(int16_t units, float speedFactor)
{

	if(units < 0) units = 0;
	if(speedFactor <= 0) speedFactor = 1;

	int16_t _speed = (1000000 / (STEPPERS_STEPS_PER_SECOND * speedFactor));

	int32_t steps = STEPPERS_LINE_STEPS * units;

	uint8_t stepCounter1 = 0;
	int8_t stepCounter2 = numSteps - 1;

	for (int32_t i = 0; i < steps; i++)
	{
		if (stepCounter1 >= numSteps) stepCounter1 = 0;
		if (stepCounter2 < 0) stepCounter2 = numSteps - 1;
		motorStepLeft(step_pattern[stepCounter1]);
		motorStepRight(step_pattern[stepCounter2]);
		delayMicroseconds(_speed);
		stepCounter1++;
		stepCounter2--;
	}

	motorStepLeft(0);
	motorStepRight(0);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::_moveBackward(int16_t units, float speedFactor)
{

	if(units < 0) units = 0;
	if(speedFactor <= 0) speedFactor = 1;

	int16_t _speed = (1000000 / (STEPPERS_STEPS_PER_SECOND * speedFactor));

	int32_t steps = STEPPERS_LINE_STEPS * units;

	uint8_t stepCounter1 = 0;
	int8_t stepCounter2 = numSteps - 1;

	for (int32_t i = 0; i < steps; i++)
	{
		if (stepCounter1 >= numSteps) stepCounter1 = 0;
		if (stepCounter2 < 0) stepCounter2 = numSteps - 1;
		motorStepRight(step_pattern[stepCounter1]);
		motorStepLeft(step_pattern[stepCounter2]);
		delayMicroseconds(_speed);
		stepCounter1++;
		stepCounter2--;
	}

	motorStepLeft(0);
	motorStepRight(0);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::motorStepLeft(uint8_t pattern)
{
    digitalWrite(STEPPERS_MOTOR_LEFT_IN1, bitRead(pattern, 0));
    digitalWrite(STEPPERS_MOTOR_LEFT_IN2, bitRead(pattern, 1));
    digitalWrite(STEPPERS_MOTOR_LEFT_IN3, bitRead(pattern, 2));
    digitalWrite(STEPPERS_MOTOR_LEFT_IN4, bitRead(pattern, 3));
}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::motorStepRight(uint8_t pattern)
{
    digitalWrite(STEPPERS_MOTOR_RIGHT_IN1, bitRead(pattern, 0));
    digitalWrite(STEPPERS_MOTOR_RIGHT_IN2, bitRead(pattern, 1));
    digitalWrite(STEPPERS_MOTOR_RIGHT_IN3, bitRead(pattern, 2));
    digitalWrite(STEPPERS_MOTOR_RIGHT_IN4, bitRead(pattern, 3));
}

#endif

//////////////////////////////////////////////////////////////////////
/*
 * LED FUNCTIONS
 */
//////////////////////////////////////////////////////////////////////

#ifdef SIMPLE_LED_PIN

void EscornabotExtension::ledOn()
{
    digitalWrite(SIMPLE_LED_PIN, HIGH);
}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::ledOff()
{
    digitalWrite(SIMPLE_LED_PIN, LOW);
}


//////////////////////////////////////////////////////////////////////

void EscornabotExtension::blinkingLed(int16_t num)
{
	if(num < 0) num = 0;

	for (int16_t i = 0; i < num; i++)
	{
		digitalWrite(SIMPLE_LED_PIN, HIGH);
		delay(200);
		digitalWrite(SIMPLE_LED_PIN, LOW);
		delay(200);
	}
}

#endif

//////////////////////////////////////////////////////////////////////

#if defined(SIMPLE_LED_PIN) || defined (KEYPAD_LED_PIN_UP) || defined(KEYPAD_LED_PIN_RIGHT) || defined(KEYPAD_LED_PIN_DOWN) || defined(KEYPAD_LED_PIN_LEFT) || defined(KEYPAD_LED_PIN_GO)

void EscornabotExtension::turnLedOnOff(uint8_t state,String ledPin)
{
	uint8_t _pin = 13;

	if (ledPin.length() == 0)
		return;

	if(state < 0) state = 0;
	if(state > 1) state = 1;

	if (ledPin =="ONBOARD")
		_pin = SIMPLE_LED_PIN;
	else if (ledPin =="UP")
		_pin = KEYPAD_LED_PIN_UP;
	else if (ledPin =="RIGHT")
		_pin = KEYPAD_LED_PIN_RIGHT;
	else if (ledPin =="DOWN")
		_pin = KEYPAD_LED_PIN_DOWN;
	else if (ledPin =="LEFT")
		_pin = KEYPAD_LED_PIN_LEFT;
	else if (ledPin =="GO")
		_pin = KEYPAD_LED_PIN_GO;

	digitalWrite(_pin, state);

}


//////////////////////////////////////////////////////////////////////

void EscornabotExtension::blinkingLed(String ledPin,int16_t num)
{
	uint8_t _pin = 13;

	if (ledPin.length() == 0)
		return;

	if(num < 0) num = 0;

	if (ledPin =="ONBOARD")
		_pin = SIMPLE_LED_PIN;
	else if (ledPin =="UP")
		_pin = KEYPAD_LED_PIN_UP;
	else if (ledPin =="RIGHT")
		_pin = KEYPAD_LED_PIN_RIGHT;
	else if (ledPin =="DOWN")
		_pin = KEYPAD_LED_PIN_DOWN;
	else if (ledPin =="LEFT")
		_pin = KEYPAD_LED_PIN_LEFT;
	else if (ledPin =="GO")
		_pin = KEYPAD_LED_PIN_GO;

	for (int16_t i = 0; i < num; i++)
	{
		digitalWrite(_pin, HIGH);
		delay(200);
		digitalWrite(_pin, LOW);
		delay(200);
	}
}

#endif

//////////////////////////////////////////////////////////////////////
/*
 * SOUND FUNCTIONS
 */
//////////////////////////////////////////////////////////////////////

#ifdef BUZZER_PIN

void EscornabotExtension::playNote(int note, int duration)
{
	if(note < 0) note = 0;
	if(duration < 0) duration = 0;

	tone(BUZZER_PIN, note, duration);
	delay(duration);
	noTone(BUZZER_PIN);
}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::playMelody(int8_t melody)
{
	if(melody < 0) melody = 0;

	 switch (melody) {
	      case 1:
	        melody1();
	        break;
	      case 2:
	        melody2();
	        break;
	      case 3:
	        melody3();
	        break;
	      case 4:
			melody4();
			break;
	 }

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::executeMelody(const int melody[], const int noteDurations[], const int notes)
{
   for (int thisNote = 0; thisNote < notes; thisNote++)
   {
		int noteDuration = 1000 / noteDurations[thisNote];
		tone(BUZZER_PIN, melody[thisNote], noteDuration);
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);
		noTone(BUZZER_PIN);
  }

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::melody1()
{
  const int melody[] = {NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, 0, NOTE_F7, NOTE_GS7};
  const int noteDuration[] = {9, 9, 9, 9, 9, 9, 3};

  executeMelody(melody,noteDuration,7);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::melody2()
{

  const int melody[] = {NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0, 0, NOTE_G6, 0, 0, 0};
  const int noteDuration[] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

  executeMelody(melody,noteDuration,16);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::melody3()
{

  const int melody[] = {NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0, NOTE_C7, NOTE_D7, NOTE_B6, 0, 0};
  const int noteDuration[] = {9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

  executeMelody(melody,noteDuration,15);

}

//////////////////////////////////////////////////////////////////////

void EscornabotExtension::melody4()
{

	const int melody[] = {NOTE_B7, NOTE_AS7, NOTE_A7, NOTE_GS7, NOTE_G7, NOTE_FS7,
					NOTE_F7, NOTE_E7, NOTE_DS7, NOTE_D7, NOTE_CS7, NOTE_D7,
					NOTE_CS7, NOTE_C7, NOTE_B6, NOTE_AS6, NOTE_A6, NOTE_GS6,
					NOTE_G6, NOTE_FS6, NOTE_F6, NOTE_E6, NOTE_DS6, NOTE_D6};

	const int noteDuration[] = {7, 7, 7, 12, 12, 12, 16, 16, 7, 7, 7, 12, 12,
							   12, 16, 16, 16, 16, 16, 16, 16, 12, 12, 11};

    executeMelody(melody,noteDuration,24);

}

#endif

//////////////////////////////////////////////////////////////////////
/*
 * BUTTON SET ANALOG FUNCTIONS
 */
//////////////////////////////////////////////////////////////////////

#ifdef BUTTONS_ANALOG

bool EscornabotExtension::buttonIsPressed(String button)
{

	if (button.length() == 0)
		return false;

	int8_t buttonPressed = scanButtons();
	if (buttonPressed < 0)
		return false;

	if (button == bs_analog_text[buttonPressed])
		return true;
	else
		return false;

}

/////////////////////////////////////////////////////////////////////

int8_t EscornabotExtension::scanButtons()
{
	const uint8_t TOLERANCE_MARGIN = 10;

    int16_t value = analogRead(BS_ANALOG_PIN);
    int16_t _diff,diff;

    for (uint8_t i=0; i<5; i++)
    {
		_diff = (value - bs_analog_value[i]);
		diff = abs(_diff);
		if (diff < TOLERANCE_MARGIN)
			return i;
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////

#endif


//////////////////////////////////////////////////////////////////////
/*
 * ULTRASONIC SENSOR FUNCTIONS
 */
//////////////////////////////////////////////////////////////////////

#if ULTRASONIC_SENSOR


long EscornabotExtension::getDistance()
{

	long duration, distance;

    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(4);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN,HIGH);

    distance = duration /((29.2)*2);   // distance in cm

    // No Echo
    if (distance == 0)
      distance = 500;

    return distance;

}

bool EscornabotExtension::obstacleDetected(long limitDistance){

	if (limitDistance <= 0)
		return false;

	long distance = getDistance();
	if (distance <= limitDistance)
	  return true;
	else
	  return false;

}

/////////////////////////////////////////////////////////////////////

#endif

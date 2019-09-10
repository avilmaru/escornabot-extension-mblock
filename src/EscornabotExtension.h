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


#ifndef ESCORNABOTEXTENSION_H_
#define ESCORNABOTEXTENSION_H_

#include "Configuration.h"
#include "Pitches.h"

#ifdef ENGINE_TYPE_STEPPERS
const uint8_t numSteps = 8;
const uint8_t step_pattern[8] = {
		B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001};
#endif

#ifdef BUTTONS_ANALOG

const uint16_t bs_analog_value[5] = {
		BS_ANALOG_VALUE_UP,BS_ANALOG_VALUE_DOWN,BS_ANALOG_VALUE_LEFT,BS_ANALOG_VALUE_RIGHT,BS_ANALOG_VALUE_GO
};

#if CHANGE_DIRECTION
const String bs_analog_text[5] = {
		"DOWN","UP","RIGHT","LEFT","GO"
};
#else
const String bs_analog_text[5] = {
		"UP","DOWN","LEFT","RIGHT","GO"
};
#endif

#endif

class EscornabotExtension {

public:

	EscornabotExtension();
	virtual ~EscornabotExtension();

	// Stepper Engine
	#ifdef ENGINE_TYPE_STEPPERS
	void turnRight(int16_t degrees, float speedFactor);
	void turnLeft(int16_t degrees, float speedFactor);
	void moveForward(int16_t units, float speedFactor);
	void moveBackward(int16_t units, float speedFactor);
	#endif

	// LED Pin
	#ifdef SIMPLE_LED_PIN
	void ledOn();
	void ledOff();
	void blinkingLed(int16_t num);
	#endif

	#if defined(SIMPLE_LED_PIN) || defined (KEYPAD_LED_PIN_UP) || defined(KEYPAD_LED_PIN_RIGHT) || defined(KEYPAD_LED_PIN_DOWN) || defined(KEYPAD_LED_PIN_LEFT) || defined(KEYPAD_LED_PIN_GO)
	void turnLedOnOff(uint8_t state,String ledPin);
	void blinkingLed(String ledPin,int16_t num);
	#endif


	 // Buzzer
	#ifdef BUZZER_PIN
	void playNote(int note, int duration);
	void playMelody(int8_t melody);
	#endif

	// Button Set Analog
	#ifdef BUTTONS_ANALOG
	bool buttonIsPressed(String button);
	#endif

	// Ultrasonic sensor
	#if ULTRASONIC_SENSOR
	bool obstacleDetected(long limitDistance);
	#endif

private:

	// Stepper Engine
	#ifdef ENGINE_TYPE_STEPPERS
	void _moveForward(int16_t units, float speedFactor);
	void _moveBackward(int16_t units, float speedFactor);
	void motorStepLeft(uint8_t pattern);
	void motorStepRight(uint8_t pattern);
	#endif

	 // Buzzer
	#ifdef BUZZER_PIN
	void executeMelody(const int melody[], const int noteDurations[], const int notes);
	void melody1();
	void melody2();
	void melody3();
	void melody4();
	#endif

	// Button Set Analog
	#ifdef BUTTONS_ANALOG
	int8_t scanButtons();
	#endif

	// Ultrasonic sensor
	#if ULTRASONIC_SENSOR
	long getDistance();
	#endif

};

#endif /* ESCORNABOTEXTENSION_H_ */

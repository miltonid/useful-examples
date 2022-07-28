/*
  State change detection (edge detection) whith EK-TM4C123GXL LaunchPad.
  This project is arduino compatible.

 	 	
 The circuit EK-TM4C123GXL:
 * pushbutton2 attached to pin 5 from GND
 * RED_LED attached from pin 14 to ground through a 470 Ohm resistor
 * Serial communication fro report the state machine status.
 
 created  27/07/2022
 by Milton Sierra Solano

This example code is in the public domain.
 */

//Status of state machine
#define STATE_OFF 0
#define STATE_PRESSED 1
#define STATE_ON 2
#define STATE_RELEASED 3
#define STATE_PROTECTED 4


// this constant won't change:
const int16_t  buttonPin = PUSH2;    ///< the pin that the pushbutton is attached to
const int16_t ledPin = RED_LED;      ///< the pin that the LED is attached to


// Variables will change:
int16_t buttonState = 0;    ///< current state of the button
uint32_t timeStamp = 0;     ///< timeStamp in milliseconds

void stateMachine();

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
  Serial.println("State off");
  digitalWrite(ledPin, HIGH);  
}


void loop() {
  stateMachine();  
}

/**
 * @brief This function checks if the button is pressed 
 * (state ON) or not (state OFF) and for how long the 
 * button is kept pressed.
 */
void stateMachine() {
  static uint8_t cntDebounce = 0;
  static uint8_t state = 0;
  static uint32_t timeStampMsButtonPressed = 0;
  uint32_t timePressed, now;


  switch (state)
  {
  case STATE_OFF:
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      cntDebounce++;
      if (cntDebounce == 10) {
        state = STATE_PRESSED;
      }
      delay(1);    
    }else {
      cntDebounce = 0;
    }
    break;

  case STATE_PRESSED: 
    Serial.println("Button Pressed");
    cntDebounce = 0;
    timeStampMsButtonPressed = millis();
    state = STATE_ON;
    break;

  case STATE_ON: 
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
          cntDebounce++;
        if (cntDebounce == 10) {
          state = STATE_RELEASED;
        }
        delay(1);   
      }else{
        cntDebounce = 0;
      }
    break;

  case STATE_RELEASED: 
    now = millis();
    timePressed = now - timeStampMsButtonPressed;
    Serial.print("Button Released, time pressed: ");
    Serial.print(timePressed, DEC);
    Serial.println(" ms");
    state = STATE_PROTECTED;
    break;

  case STATE_PROTECTED:
    Serial.println("State Protected");
    delay(10000);   
    state = 0;
    Serial.println("State off");
    break;

  }
}

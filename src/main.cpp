#include <Arduino.h>
#include <Rotary.h> // https://github.com/brianlow/Rotary
#include <Button.h> // https://github.com/madleech/Button

Rotary r1 = Rotary(2,3);
Rotary r2 = Rotary(5,6);
Rotary r3 = Rotary(8,9);

Button b1 = Button(4); // Rotary 1 button
Button b2 = Button(7); // Rotary 2 button
Button b3 = Button(10); // Rotary 3 button

// Prototype for pollRotary (definition is further below)
// This allows us to have optional arguments in our function.
void pollRotary(Rotary &r, char *rotaryName = "Rotary", char *ccw = "CCW", char *cw = "CW", bool onlyOnStop = false, int cooldown = 1000);

unsigned char lastResult = 0x0;
unsigned long lastPoll = millis();

bool pwrPressed = false;
bool blinkLed = false;
unsigned long pwrLastPress = millis();

// let's give the Pi 2 minutes to properly halt before 
// giving the user the chance to trigger halt again...
int powerDownCycleLength = 120000;

void setup() 
{
  Serial.begin(9600);
  r1.begin(true);
  r2.begin(true);
  r3.begin(true);
  b1.begin();
  b2.begin();
  b3.begin();

  // LED: defaults to ON
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() 
{
  pollRotary(r1, "VL", "DN", "UP", false, 500);
  pollRotary(r2, "PB", "PV", "NX", true, 500);
  pollRotary(r3, "AUX", "DN", "UP", false, 500);

  if (blinkLed) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // If enough time (30s) has elapsed since pressing
  // the power button, return things to normal.
  if (blinkLed && (millis() >= pwrLastPress + powerDownCycleLength)) {
    Serial.println("Powerdown cycle complete.");
    blinkLed = false;
  }

  // Handle the rotary buttons
  if (b1.pressed()) {
    Serial.println("VL_RS");
  }
  
  if (b2.pressed()) {
    Serial.println("PB_PP");
  }

  if (b3.pressed()) {
    pwrPressed = true;
    pwrLastPress = millis();
  }

  // If it's a long press do the long press action
  if (pwrPressed && (millis() > pwrLastPress + 3000)) {
    Serial.println("PWRDN");
    pwrPressed = false;
    blinkLed = true;
    pwrLastPress = millis();
  }
  if (b3.released()) {

    // If it was just a quick press, just do the button action
    if (pwrPressed && (millis() < pwrLastPress + 500)) {
      Serial.println("AUX_GO");
    }
    pwrPressed = false;
    pwrLastPress = millis();
  }
}

void pollRotary(Rotary &r, char *rotaryName, char *ccw, char *cw, bool onlyOnStop, int cooldown)
{
  unsigned char result = r.process();
  if (result) {
    if (onlyOnStop) {
      if ((lastResult != result) || (millis() >= lastPoll + cooldown)) {
        Serial.print(rotaryName);
        Serial.print("_");
        Serial.println(result == DIR_CCW ? cw : ccw);
      }
      lastResult = result;
      lastPoll = millis();
    } else {
      Serial.print(rotaryName);
      Serial.print("_");
      Serial.println(result == DIR_CCW ? cw : ccw);
    }
  }
}

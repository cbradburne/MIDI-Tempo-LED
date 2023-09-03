#include <MIDI.h>

unsigned long int startTime;

bool counting = false;
bool bpmSet = false;
bool ledState = LOW;

int beatCount;
long int noteTime;
long int bpm;
long int bpmOLD;
long int bpmOLDtwice;

float bpmMillis = 2000;
float previousMillis = 0;


void setup() {
  //Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  usbMIDI.setHandleClock(myClock);
}

void loop() {
  usbMIDI.read();

  float currentMillis = millis();
  if (currentMillis - previousMillis >= bpmMillis) {    // If time since last tick = difference in tick time
    previousMillis = currentMillis;                       // Set last tick time to now

    if (bpmSet) {                                         // Check if BPM set
      if (ledState == LOW) {
        digitalWrite(LED_BUILTIN, ledState);                          // Turn LED off
        ledState = HIGH;                                    // flip state
      } else {
        digitalWrite(LED_BUILTIN, ledState);                          // Turn LED on
        ledState = LOW;                                     // flip state
      }
    } else {                                              // Check if BPM NOT set, 
        ledState = LOW;                                     // Set state to LOW
        digitalWrite(LED_BUILTIN, ledState);                          // Turn LED off until BPM set
    }
  }
}

void myClock() {
  if (not counting) {                                   // first time you have seen a clock message in this batch
    startTime = millis();
    counting = true;
    beatCount = 1;
  } else {
    beatCount += 1;
    if (beatCount >= 96) {
      noteTime = millis() - startTime;
      counting = false;
      bpm = 240000 / noteTime;                              // you now have the time for one note so calculate the BPM
      bpm--;

      if (bpmOLD != bpm) {                                  // If previous BPM doesn't match new BPM, set BPM to false. Turn off LED
        bpmSet = false;
        bpmOLD = bpm;                                         // Store BPM as old BPM
      } else {
        if (bpmOLDtwice != bpm) {                             // Check twice as first calculation can be incorrect
          bpmOLDtwice = bpm;
          //Serial.println(bpm);                                  // Print BPM
          bpmMillis = 30000 / bpm;                              // Store how many milliseconds between each quaver
          bpmSet = true;                                        // Set bpmSet to true so LED will flash
        }
      }
    }
  }
}
int lastState4 = LOW;
int lastState3 = LOW;

void setup() {
    Serial.begin(9600);
    
    pinMode(4, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
}

void loop() {
    int currentState4 = digitalRead(4);
    int currentState3 = digitalRead(3);

    // Check transitions for Pin 4
    if (currentState4 != lastState4) {
        if (currentState4 == HIGH) {
            Serial.println("Pin 4: LOW to HIGH");
        } else {
            Serial.println("Pin 4: HIGH to LOW");
        }
        lastState4 = currentState4; // Update state
    }

    // Check transitions for Pin 3
    if (currentState3 != lastState3) {
        if (currentState3 == HIGH) {
            Serial.println("Pin 3: LOW to HIGH");
        } else {
            Serial.println("Pin 3: HIGH to LOW");
        }
        lastState3 = currentState3; // Update state
    }

    delay(10); // Small delay to debounce
}


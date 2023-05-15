#include "grovepi.h"
#include <iostream>

using namespace GrovePi;
using namespace std;

class Buzzer {
public:
    Buzzer(int pin) {
        pin_=pin;
        pinMode(pin, OUTPUT);
    }

    void buzz() {
        digitalWrite(pin_, HIGH);
        cout << "buzzer ON" << endl;
        delay(1000);
        digitalWrite(pin_, LOW);
        cout << "buzzer OFF" << endl;
        delay(1000);
    }

private:
    int pin_;
};

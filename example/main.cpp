#include <avr-cpp/ioport.h>

int main() {
    using namespace avrcpp;

    // LED connected to port B, pin 4.
    IOPin<PortB, 4> led;
    led.setDirection(OUTPUT);
    // Start with LED enabled.
    led.set();

    // Button connected to port B, pin 5, internal pull-up.
    IOPin<PortB, 4> button;
    button.setDirection(INPUT_PULL_UP);
    // Set LED to current button value.
    led.set(button.read());

    while(1);
}


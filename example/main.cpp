#include <avr-cpp/ioport.h>

int main() {
    using namespace avrcpp;

    // LED connected to port B, pin 4.
    IOPin<PortB, 4> led_pin;
    led_pin.setDirection(OUTPUT);
    led_pin.set();

    // Button connected to port B, pin 5, internal pull-up
    IOPin<PortB, 4> button_pin;
    button_pin.setDirection(INPUT_PULL_UP);
    int button_val = button_pin.read();

    while(1);
}


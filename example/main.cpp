#include <avr-cpp/ioport.h>

int main() {
    using namespace avrcpp;

    // LED connected to port B, pin 4.
    IOPin<PortB, 4> led_pin;
    led_pin.setDirection(OUTPUT);

    // Button connected to port B, pin 5, internal pull-up.
    // Holding the button on start will turn the LED off.
    IOPin<PortB, 4> button_pin;
    button_pin.setDirection(INPUT_PULL_UP);
    if(button_pin.read())
        led_pin.set();

    while(1);
}


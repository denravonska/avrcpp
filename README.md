Proof of concept for C++ development on 8bit AVR with minimal overhead.

# Design goals
- The library should be easier to use than raw register manipulation.
- The library should not introduce overhead.

# Example

```cpp
// LED connected to port B, pin 4.
IOPin<PortB, 4> led;
led.setDirection(OUTPUT);
// Start with LED enabled.
led.set();

// Button connected to port B, pin 5, internal pull-up.
IOPin<PortB, 5> button;
button.setDirection(INPUT_PULL_UP);
// Set LED to current button value.
led.set(button.read());
```

This produces the following output when using `-Os`
```
  e4:   24 9a           sbi     0x04, 4 ; 4
  e6:   1c 9a           sbi     0x03, 4 ; 3
  e8:   25 98           cbi     0x04, 5 ; 4
  ea:   1d 9a           sbi     0x03, 5 ; 3
  ec:   83 b1           in      r24, 0x03       ; 3
  ee:   83 b1           in      r24, 0x03       ; 3
  f0:   93 b1           in      r25, 0x03       ; 3
  f2:   8f 7e           andi    r24, 0xEF       ; 239
  f4:   89 2b           or      r24, r25
  f6:   83 b9           out     0x03, r24       ; 3
```

The raw registry access counterpart usually looks something like
```cpp
// LED connected to port B, pin 4
DDRB |= _BV(DDB4); // Set output
PINB |= _BV(PINB4); // Set high

// Button connected to port B, pin 5, internal pull-up
DDRB &= ~_BV(DDB5); // Set input
PINB |= _BV(PINB5);  // Enable pull-up
// Put state of PB5 into PB4
PINB = (PINB & ~(_BV(PINB4))) | ((PINB & _BV(PINB5)) >> 1);
```

In it's raw form it tends to become unreadable, which can be somewhat mitigated by using defines to get more sane pin definitions. However, it's still difficult to read and you still can't easily pass pins around. What's worse is the output produced.

```
  e4:   24 9a           sbi     0x04, 4 ; 4
  e6:   1c 9a           sbi     0x03, 4 ; 3
  e8:   25 98           cbi     0x04, 5 ; 4
  ea:   2d 9a           sbi     0x05, 5 ; 5
  ec:   23 b1           in      r18, 0x03       ; 3
  ee:   93 b1           in      r25, 0x03       ; 3
  f0:   90 72           andi    r25, 0x20       ; 32
  f2:   89 2f           mov     r24, r25
  f4:   90 e0           ldi     r25, 0x00       ; 0
  f6:   95 95           asr     r25
  f8:   87 95           ror     r24
  fa:   92 2f           mov     r25, r18
  fc:   9f 7e           andi    r25, 0xEF       ; 239
  fe:   89 2b           or      r24, r25
 100:   83 b9           out     0x03, r24       ; 3
```
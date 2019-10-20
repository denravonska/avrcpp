Proof of concept for C++ development on 8bit AVR with minimal overhead.

# Design goals
- The library should be easier to use than raw register manipulation.
- The library should not introduce overhead.

# Example
## Pins on the same port
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
  ee:   88 23           and     r24, r24
  f0:   11 f0           breq    .+4             ; 0xf6 <main+0x12>
  f2:   1c 9a           sbi     0x03, 4 ; 3
  f4:   01 c0           rjmp    .+2             ; 0xf8 <main+0x14>
  f6:   1c 98           cbi     0x03, 4 ; 3
```

The raw registry access counterpart usually looks something like
```cpp
// LED connected to port B, pin 4
DDRB = _BV(DDB4); // Set output
PINB = _BV(PINB4) // Set high
     | _BV(PINB5); // Enable pull-up

if (PINB & _BV(PINB4))
    PINB |= _BV(PINB5);
else
    PINB &= ~_BV(PINB5);
```

While producing smaller output, it's still difficult to read, you can't easily pass pins around. 

```
  e4:   80 e1           ldi     r24, 0x10       ; 16
  e6:   84 b9           out     0x04, r24       ; 4
  e8:   80 e3           ldi     r24, 0x30       ; 48
  ea:   83 b9           out     0x03, r24       ; 3
  ec:   1c 9b           sbis    0x03, 4 ; 3
  ee:   02 c0           rjmp    .+4             ; 0xf4 <main+0x10>
  f0:   1d 9a           sbi     0x03, 5 ; 3
  f2:   01 c0           rjmp    .+2             ; 0xf6 <main+0x12>
  f4:   1d 98           cbi     0x03, 5 ; 3
```

## Pins on different ports
C++
```
  e4:   24 9a           sbi     0x04, 4 ; 4
  e6:   1c 9a           sbi     0x03, 4 ; 3
  e8:   3d 98           cbi     0x07, 5 ; 7
  ea:   35 9a           sbi     0x06, 5 ; 6
  ec:   86 b1           in      r24, 0x06       ; 6
  ee:   88 23           and     r24, r24
  f0:   11 f0           breq    .+4             ; 0xf6 <main+0x12>
  f2:   1c 9a           sbi     0x03, 4 ; 3
  f4:   01 c0           rjmp    .+2             ; 0xf8 <main+0x14>
  f6:   1c 98           cbi     0x03, 4 ; 3
```

C
```
  e4:   80 e1           ldi     r24, 0x10       ; 16
  e6:   84 b9           out     0x04, r24       ; 4
  e8:   83 b9           out     0x03, r24       ; 3
  ea:   80 e2           ldi     r24, 0x20       ; 32
  ec:   86 b9           out     0x06, r24       ; 6
  ee:   1c 9b           sbis    0x03, 4 ; 3
  f0:   02 c0           rjmp    .+4             ; 0xf6 <main+0x12>
  f2:   35 9a           sbi     0x06, 5 ; 6
  f4:   01 c0           rjmp    .+2             ; 0xf8 <main+0x14>
  f6:   35 98           cbi     0x06, 5 ; 6
```
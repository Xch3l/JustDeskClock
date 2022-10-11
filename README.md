# JustDeskClock

I needed a desk clock that did nothing else than just tell time without spending a lot of money. Also recycling an old LED display from a VCR I scavenged long time ago. Remember VCRs? They were alright, until the tape got tangled in the heads and you had to pull, unwind and rewind carefully so you don't ruin your favorite Finding Nemo VHS cassette. &#x1F972;

**NOTE:** There's a little bit of Binary Coded Decimals (BCD) and bit fiddling (boolean operations).

Parts list
----------

- Board: For this simple project I used a 5V "Sparkfun" ProMicro (clone, sadly) that I had collecting dust (literally!). But any Arduino-powered device and a little tweaking should be good enough.
- 3 1K&Omega; (or so) resistors: Should be tied to `Ground`, to pull down our button inputs.
- Display: This weird LED display. In case you don't have it (most likely), the commonly used 4 digit display should be enough with a little rewiring and some minor code changes (left as an excersice to the reader! &#x1F609; )
- Jumper wires
- Breadboard: Mmmmmm, _board of bread_ &#x1F924;

Wiring
------

Should be easy to follow the diagram:

![JustDeskClock_board](https://user-images.githubusercontent.com/17131442/195031816-6dcd27da-aefa-41d2-9a52-b6b9e725057a.png)

- A3 &rarr; COM0
- A2 &rarr; COM1
- A1 &rarr; COM2
- A0 &rarr; COM3
- 15 &rarr; COM4
- 2 to 9 &rarr; SEG0 to SEG7

Buttons: All buttons should be tied to Ground with a resistor to avoid wrong readings and the clock going crazy. Set them to VCC to activate them ("active high")

- 14 &rarr; Reset clock: Resets the clock to `12:00`
- 16 &rarr; Set hours
- 10 &rarr; Set minutes

LED Display pinout
------------------

For the sake of completeness, I also made the pinout for the display:

![JustDeskClock_displayPinout](https://user-images.githubusercontent.com/17131442/195034788-ccb5ffc3-1b74-4707-96a3-8ec47046f4bf.png)

|      | SEG0 | SEG1 | SEG2 | SEG3 | SEG4 | SEG5 | SEG6 | SEG7 |
|------|------|------|------|------|------|------|------|------|
| COM0 |  a   |  b   |  c   |  d   |  e   |  f   |  g   |  h   |
| COM1 |  a   |  b   |  c   |  d   |  e   |  f   |  g   |  h   |
| COM2 |  a   |  b   |  c   |  d   |  e   |  f   |  g   |  h   |
| COM3 |  a   |  b   |  c   |  d   |  e   |  f   |  g   |  h   |
| COM4 | `Hi-Fi` | Upper dot | Minus | `VCR` | Lower dot | `TIMER` | Top-middle bar of 2nd digit | "Tape" icon |

Each `COMn` (common) line connects a group of LEDs and each `SEGn` connects to each segment of a digit as in a traditional Seven Segment display, where `h` is the extra segment.

- COM0: Hours (10s)
- COM1: Hours (1s)
- COM2: Minutes (10s)
- COM3: Minutes (1s)
- COM4: Misc. symbols (refer to the table above)

Cat tax
-------

&#x1F63A;

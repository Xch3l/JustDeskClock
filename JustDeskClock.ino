/* --------------------------------------------
**  JustDeskClock
**    Just a simple desk clock with no extra
**    functionality other than telling time.
**
**                                Xch3l, 2022
** --------------------------------------------
**  Target board:
**    Sparkfun Pro Micro, 5V/16MHz
**
**  Wiring:
**    A3, A2, A1, A0, 15  -  COM0-COM4
**    2-9  -  SEG0-SEG7
**    14  -  Reset clock
**    16  -  Set hours
**    10  -  Set minutes
** --------------------------------------------
*/

byte bits[5]; // LED Display
long past = 0;

// Time keeping bytes
byte hours = 0x12; // as BCD
byte minutes = 0x00; // same
byte seconds = 0; // not BCD
byte dots = 0; // "boolean"

// Current button state
byte buttons = 0;

// LED Display commons
const byte coms[5] = {A0, A1, A2, A3, 15};

// 8-Segment bitmasks
const byte digits[16] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, // 0 thru 9
  0, 0, 0, 0, 0, 0 // Unused (10 to 15)
};

// Misc. symbol bits (COM4)
const byte Sym_HiFi  = 0x01;
const byte Sym_Dot1  = 0x02;
const byte Sym_Minus = 0x04;
const byte Sym_VCR   = 0x08;
const byte Sym_Dot2  = 0x10;
const byte Sym_Timer = 0x20;
const byte Sym_T     = 0x40;
const byte Sym_REC   = 0x80;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, INPUT);
  pinMode(16, INPUT);
  pinMode(14, INPUT);

  for(byte i = 0; i < 5; i++) {
    pinMode(coms[i], OUTPUT);
    digitalWrite(coms[i], LOW);
  }

  resetTime();
  setBits(0);
}

void loop() {
  byte i;

  checkButtons(); // Read buttons and act on them
  fixDigits(); // Fix internal digits into BCD

  for(i = 0; i < 5; i++) {
    digitalWrite(coms[i], HIGH); // Enable group

    if(i == 4)
      setBits(bits[i]); // Light up last group raw
    else
      setBits(digits[bits[i] & 15]); // Light up each digit's pattern

    setBits(0); // Turn all off
    digitalWrite(coms[i], LOW); // Disable group
  }

  // Check if half-second passed
  long t = millis();
  if((t - past) >= 500) {
    past = t;

    // Toggle ':' symbol (two LEDs, actually)
    bits[4] ^= Sym_Dot1 | Sym_Dot2;

    // Increase secconds every two flashes
    if(dots = !dots)
      seconds++;
  }
}

// Resets time to 12:00
void resetTime() {
  hours = 0x12;
  minutes = 0x00;
  resetCounters();
}

// Resets all counters
void resetCounters() {
  seconds = 0;
  dots = 0;

  past = millis();
  bits[4] |= Sym_Dot1 | Sym_Dot2;
}

void checkButtons() {
  byte old = buttons; // keep track of previous state
  byte bdown; // current state just this moment

  // Read button inputs
  buttons = 0;
  if(digitalRead(10)) buttons |= 1;
  if(digitalRead(16)) buttons |= 2;
  if(digitalRead(14)) buttons |= 4;

  // Mask out older states
  bdown = (old ^ buttons) & buttons;

  if(bdown) {
    if(bdown & 1) // Button at pin 10
      minutes++;

    if(bdown & 2) // Button at pin 16
      hours++;

    if(bdown & 4) { // Button at pin 14
      resetTime();
      return; // no need to reset twice
    }

    resetCounters();
  }
}

// Fixes data bytes to Binary Coded Decimals (BCD)
// and maps them to display bits
void fixDigits() {
  if(seconds == 60) {
    seconds = 0;
    minutes++;
  }

  if((minutes & 15) == 10)
    minutes += 6;

  if(minutes == 0x60) {
    minutes = 0;
    hours++;
  }

  if((hours & 15) == 10)
    hours += 6;

  if(hours == 0x13) {
    hours = 1;
    bits[4] ^= Sym_Minus; // toggle "AM" ""symbol""
  }

  bits[0] = minutes & 15;
  bits[1] = minutes >> 4;

  bits[2] = hours & 15;
  bits[3] = hours >> 4;
}

void setBits(byte x) {
  x = ~x; // Invert value (common cathode)

  digitalWrite(2, x & 0x01); // a
  digitalWrite(3, x & 0x02); // b
  digitalWrite(4, x & 0x04); // c
  digitalWrite(5, x & 0x08); // d
  digitalWrite(6, x & 0x10); // e
  digitalWrite(7, x & 0x20); // f
  digitalWrite(8, x & 0x40); // g
  digitalWrite(9, x & 0x80); // h
}

const int pwmCool = 9;   // Cool White PWM pin
const int pwmWarm = 10;  // Warm White PWM pin

int brightness = 100;  // Full brightness (start at 255)
int blendValue = 0;    // Start with cool white (0 for cool, 1023 for warm)
int sweepSpeed = 5;   // Speed of the sweeping effect (lower is faster)

void setup() {
  // Change the PWM frequency for pins 9 and 10 (Timer1)
  // Setting the prescaler to 8 will give a PWM frequency of ~10kHz
  TCCR1B = TCCR1B & 0b11111000 | 0x02;  // Set prescaler to 8

  pinMode(pwmCool, OUTPUT);
  pinMode(pwmWarm, OUTPUT);
}

void loop() {
  // Sweep from cool white to warm white while maintaining half brightness
  for (blendValue = 0; blendValue <= 1023; blendValue++) {
    // Map blendValue to PWM outputs for cool and warm LEDs
    int blendCool = map(blendValue, 0, 1023, brightness, 0);  // Cool decreases
    int blendWarm = map(blendValue, 0, 1023, 0, brightness);  // Warm increases

    // Write PWM values to the MOSFETs
    analogWrite(pwmCool, blendCool);
    analogWrite(pwmWarm, blendWarm);

    delay(sweepSpeed);  // Delay for smooth sweeping effect
  }

  // Now reverse the sweep from warm white back to cool white
  for (blendValue = 1023; blendValue >= 0; blendValue--) {
    // Map blendValue to PWM outputs for cool and warm LEDs
    int blendCool = map(blendValue, 0, 1023, brightness, 0);  // Cool decreases
    int blendWarm = map(blendValue, 0, 1023, 0, brightness);  // Warm increases

    // Write PWM values to the MOSFETs
    analogWrite(pwmCool, blendCool);
    analogWrite(pwmWarm, blendWarm);

    delay(sweepSpeed);  // Delay for smooth sweeping effect
  }
}

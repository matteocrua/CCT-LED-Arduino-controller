// Pin definitions
const int pwmCool = 9;     // PWM pin for Cool White
const int pwmWarm = 10;    // PWM pin for Warm White

// Control variables
int brightness = 128;  // Overall brightness (0-255)
int blendValue = 512;  // Blending (0-1023) -- 0 = full cool, 1023 = full warm

void setup() {
  // Change the PWM frequency for pins 9 and 10 (Timer1)
  // Setting the prescaler to 8 will give a PWM frequency of ~10kHz
  TCCR1B = TCCR1B & 0b11111000 | 0x02;  // Set prescaler to 8

  pinMode(pwmCool, OUTPUT); // Set Cool White as PWM output
  pinMode(pwmWarm, OUTPUT); // Set Warm White as PWM output
  
  Serial.begin(9600); // Start serial communication
  Serial.println("Enter commands to control the LEDs:");
  Serial.println("B <value> - Set brightness (0-255)");
  Serial.println("L <value> - Set blend (0-1023)");
}

void loop() {
  // Check if data is available in the Serial buffer
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the incoming command
    
    // Parse the command
    if (command.startsWith("B ")) {
      brightness = command.substring(2).toInt(); // Extract and set brightness
      brightness = constrain(brightness, 0, 255); // Ensure it's within range
      Serial.print("Brightness set to: ");
      Serial.println(brightness);
    } else if (command.startsWith("L ")) {
      blendValue = command.substring(2).toInt(); // Extract and set blend
      blendValue = constrain(blendValue, 0, 1023); // Ensure it's within range
      Serial.print("Blend set to: ");
      Serial.println(blendValue);
    } else {
      Serial.println("Invalid command. Use 'B <value>' or 'L <value>'.");
    }
  }

  // Map blendValue to PWM outputs for cool and warm LEDs
  int blendCool = map(blendValue, 0, 1023, brightness, 0); // Cool decreases
  int blendWarm = map(blendValue, 0, 1023, 0, brightness); // Warm increases

  // Write PWM values to the MOSFETs
  analogWrite(pwmCool, blendCool);
  analogWrite(pwmWarm, blendWarm);

  // Short delay to allow for serial communication
  delay(50);
}

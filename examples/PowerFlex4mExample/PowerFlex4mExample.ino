#include <IMC.h>

// Create a motor object
PowerFlex4m motor1 = PowerFlex4m();

float new_frequency = 60;
bool new_direction = MOTOR_FORWARD;

void setup() {
  // Initialise the starting variables
  Serial.begin(9600);
  while (!Serial);
  motor1.init(100);
}

void loop() {
  motor1.stop();
  delay(50);
  motor1.frequency(new_frequency); // Set frequency in Hz
  delay(500);
  motor1.direction(new_direction);
  motor1.start();
  motor1.setRelay(new_direction);
  delay(2000);
  new_direction = !new_direction;
  new_frequency = new_frequency + 0.5;
  if (new_frequency > 80) new_frequency = 40;
}

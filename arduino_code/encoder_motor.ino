#include <Arduino.h>
#include <Encoder.h>

#define MOTOR1_IN1 6    // Input 1 pin for motor 1
#define MOTOR1_IN2 10   // Input 2 pin for motor 1
#define MOTOR2_IN1 9    // Input 1 pin for motor 2
#define MOTOR2_IN2 5    // Input 2 pin for motor 2
#define MOTOR1_EN 11     // Enable pin for motor 2
#define MOTOR2_EN 3     // Enable pin for motor 2

#define ENCODER1_PIN1 A4  // Example pin for encoder 1
#define ENCODER1_PIN2 A5  // Example pin for encoder 1
#define ENCODER2_PIN1 A3   // Example pin for encoder 2
#define ENCODER2_PIN2 A2   // Example pin for encoder 2

Encoder encoder1(ENCODER1_PIN1, ENCODER1_PIN2);
Encoder encoder2(ENCODER2_PIN1, ENCODER2_PIN2);

String receivedCommand; // Global variable to store received command

void setup() {
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);


  // Initialize serial communication
  Serial.begin(57600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  Serial.println("Serial communication initialized");
}

void loop() {
  // Check if data is available to read from serial port
  if (Serial.available() > 0) {
    // Read the received command
    receivedCommand = Serial.readStringUntil('\n');
    Serial.print("Received command: ");
    Serial.println(receivedCommand);
    
    // Process the received command
    processCommand(receivedCommand);
  }

  // You can add other tasks or operations here
}

void processCommand(String command) {
  // Find the position of the comma
  int commaPosition = command.indexOf(',');

  // Extract left and right motor speeds from the received command
  String leftSpeedStr = command.substring(0, commaPosition);
  String rightSpeedStr = command.substring(commaPosition + 1);

  // Convert string values to floating-point numbers
  float leftSpeedValue = leftSpeedStr.toFloat();
  float rightSpeedValue = rightSpeedStr.toFloat();

  // Read encoder values
  long encoder1Value = encoder1.read();
  long encoder2Value = encoder2.read();

  // Print the encoder values for debugging
  Serial.print("Encoder1: ");
  Serial.println(encoder1Value);
  Serial.print("Encoder2: ");
  Serial.println(encoder2Value);

  // Control motors based on the received speeds
  // Assuming positive speed values mean forward and negative mean backward

  // Motor 1 control
  if (leftSpeedValue > 0) {
    // Forward
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
    analogWrite(MOTOR1_EN, 100); 
  } else if (leftSpeedValue < 0) {
    // Backward
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
    analogWrite(MOTOR1_EN, 100);
  } else {
    // Stop
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, LOW);
  }

  // Motor 2 control
  if (rightSpeedValue > 0) {
    // Forward
    digitalWrite(MOTOR2_IN1, HIGH);
    digitalWrite(MOTOR2_IN2, LOW);
    analogWrite(MOTOR2_EN, 100);
  } else if (rightSpeedValue < 0) {
    // Backward
    digitalWrite(MOTOR2_IN1, LOW);
    digitalWrite(MOTOR2_IN2, HIGH);
    analogWrite(MOTOR2_EN, 100);
  } else {
    // Stop
    digitalWrite(MOTOR2_IN1, LOW);
    digitalWrite(MOTOR2_IN2, LOW);
  }

  // Delay for 1 second
  delay(500);

  // Stop the motors after the delay
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
  analogWrite(MOTOR1_EN, 0); // stop motor 1
  analogWrite(MOTOR2_EN, 0); // stop motor 2
  
}

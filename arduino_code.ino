#include <Servo.h>

// Pin definitions for ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 3

// Create servo objects
Servo flap1;
Servo flap2;

// Variables for ultrasonic sensor
long duration;
float distance;

void setup() {
  Serial.begin(9600);
  
  // Attach servos to pins
  flap1.attach(9);   // Flap1 servo motor
  flap2.attach(10);  // Flap2 servo motor
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize servo positions
  flap1.write(140);  // Initial position for flap1
  flap2.write(86);   // Initial position for flap2
  
  Serial.println("Waste Segregation System Ready");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    if (command == "read") {
      // Read ultrasonic sensor and send distance back
      float dist = readUltrasonicDistance();
      Serial.println(dist);
    }
    else if (command == "bin1") {
      // Biodegradable waste bin
      // Flap1 movement
      for (int pos = 140; pos >= 80; pos--) {
        flap1.write(pos);
        delay(15);
      }
      delay(3000);
      for (int pos = 80; pos <= 140; pos++) {
        flap1.write(pos);
        delay(15);
      }
      Serial.println("DONE");
    }    
    else if (command == "bin2") {
      // Non-biodegradable waste bin
      // Flap2 movement
      for (int pos = 86; pos <= 110; pos+=5) {
        flap2.write(pos);
        delay(15);
      }
      delay(1000);
      // Flap1 movement
      for (int pos = 140; pos <= 168; pos++) {
        flap1.write(pos);
        delay(15);
      }
      delay(5000);
      for (int pos = 110; pos >= 86; pos-=0.05) {
        flap2.write(pos);
        delay(15);
      }
      for (int pos = 168; pos >= 140; pos--) {
        flap1.write(pos);
        delay(15);
      }
      
      Serial.println("DONE");
    }
    else if (command == "bin3") {
      // Chemical/unknown waste bin
      // Flap1 movement
      for (int pos = 140; pos <= 168; pos++) {
        flap1.write(pos);
        delay(15);
      }
      delay(5000);
      for (int pos = 168; pos >= 140; pos--) {
        flap1.write(pos);
        delay(15);
      }
      
      Serial.println("DONE");
    }
  }
}

// Function to read distance from ultrasonic sensor
float readUltrasonicDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse to trigger pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin with timeout (e.g., 30,000 microseconds = 5   meters max range)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout
  
  if (duration == 0) {
    // No echo received (timeout), return a large number
    return 999.0;
  }
  
  // Calculate distance
  // Speed of sound is 343 m/s or 0.0343 cm/μs
  // Distance = (time × speed) / 2
  distance = (duration * 0.0343) / 2;
  
  return distance;
}

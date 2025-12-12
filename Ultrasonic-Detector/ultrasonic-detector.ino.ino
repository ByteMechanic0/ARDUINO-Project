// Ultrasonic sensor
const int TRIG = 3;
const int ECHO = 2;

// Other components
const int ledGreen = 7;
const int ledRed = 8;
const int buzzer = 9;

// Distance thresholds in cm
int distance_min = 5;    // below this distance ignore
int distance_max = 50;   // beyond this distance do not trigger

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int distance = readUltrasonic(TRIG, ECHO);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance >= distance_min && distance <= distance_max) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
    dynamicAlarm(distance);
  } 
  else {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    noTone(buzzer);
  }

  delay(50);
}

// Read the ultrasonic sensor and return distance in cm
int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 999; // No echo → huge distance
  }

  return duration / 58; // convert to cm
}

// Dynamic alarm: closer object → higher pitch and faster beeps
void dynamicAlarm(int distance) {
  int frequency;
  int pauseTime;

  // Frequency between 1500 and 3500 Hz depending on distance
  frequency = map(distance, distance_max, distance_min, 1500, 3500);

  // Pause between beeps between 200ms and 20ms depending on distance
  pauseTime = map(distance, distance_max, distance_min, 200, 20);

  tone(buzzer, frequency);
  delay(pauseTime);
  noTone(buzzer);
  delay(pauseTime);
}
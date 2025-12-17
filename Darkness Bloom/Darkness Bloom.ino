const int LIGHT_SENSOR_PIN = A0;
const int LED_PIN = 3;

// --- tuning ---
const int LIGHT_THRESHOLD = 450;   // adjust for your room
const int HYSTERESIS = 40;         

// smoothing
float smoothedLight = 0;
float alpha = 0.06;

// LED control
float brightness = 0;
float targetBrightness = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int rawLight = analogRead(LIGHT_SENSOR_PIN);

  // Smooth ambient light
  smoothedLight = smoothedLight * (1 - alpha) + rawLight * alpha;

  // Decide target brightness with hysteresis
  if (smoothedLight < (LIGHT_THRESHOLD - HYSTERESIS)) {
    // Darkness → bloom
    targetBrightness = map(smoothedLight, 0, LIGHT_THRESHOLD, 180, 30);
  } 
  else if (smoothedLight > (LIGHT_THRESHOLD + HYSTERESIS)) {
    // Light → fade out completely
    targetBrightness = 0;
  }

  targetBrightness = constrain(targetBrightness, 0, 255);

  // Elegant fade
  brightness += (targetBrightness - brightness) * 0.04;

  analogWrite(LED_PIN, (int)brightness);
  delay(25);
}

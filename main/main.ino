int fsrPin = 0;
int ledPin = 1;
float fsrReading;
int ledLevel;
float fsrForce;
float fsrVoltage;
float fsrResistance;
float fsrConductance;
float array[10];

float average(float* array, int len) {
  long sum = 0L;  // sum will be larger than an item, long for safety.
  for (int j = 0; j < len; j++)
    sum += array[j];
  return ((float)sum) / len;  // average will be fractional, so float may be appropriate.
}

void setup(void) {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop(void) {
  fsrReading = analogRead(fsrPin);
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  // Serial.print("mV = ");
  // Serial.println(fsrVoltage);
  // Serial.print(",");
  if (Serial.available()) {
    String input = Serial.readString();
    Serial.print("You typed: ");
    Serial.println(input);
  }

  if (Serial.readString() == "start") {
    for (int i = 0; i <= 10; i++) {

      // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
      fsrResistance = 5000 - fsrVoltage;
      fsrResistance *= 10000;
      fsrResistance /= fsrVoltage;

      fsrConductance = 1000000;
      fsrConductance /= fsrResistance;

      // Ffsr = ((Vcc - V) * R) / V
      fsrForce = fsrConductance / 80;
      //Serial.print("Force in Newtons: ");
      array[i] = fsrForce;
    }
    Serial.println(average(array, 10));
    delay(10000);

  } else {
    // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
    fsrResistance = 5000 - fsrVoltage;
    fsrResistance *= 10000;
    fsrResistance /= fsrVoltage;

    fsrConductance = 1000000;
    fsrConductance /= fsrResistance;

    // Ffsr = ((Vcc - V) * R) / V
    fsrForce = fsrConductance / 80;
    //Serial.print("Force in Newtons: ");
    Serial.println(fsrForce);
  }
  ledLevel = map(fsrReading, 0, 1023, 0, 255);
  analogWrite(ledPin, ledLevel);
  delay(200);
}
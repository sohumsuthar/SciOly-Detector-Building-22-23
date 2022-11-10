int fsrPin = 0;
int ledPin = 1;
float fsrReading;
int ledLevel;
float fsrForce;
float fsrVoltage;
float fsrResistance;
float fsrConductance;

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


  if (fsrVoltage == 0) {
    Serial.println("No pressure");
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
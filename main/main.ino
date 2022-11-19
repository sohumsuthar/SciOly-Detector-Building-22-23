int fsrPin = 0;
int ledR = 7;
int ledG = 4;
int ledB = 2;
float fsrReading;
int ledLevel;
float fsrForce;
float fsrVoltage;
float fsrResistance;
float fsrConductance;
float thres1 = 3000; 
float thres2 = 2000;
float thres3 = 1000;

void setup(void) {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  analogReference(DEFAULT);
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
    if (fsrVoltage >= thres1) {
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, LOW);
    } else if (fsrVoltage >= thres2) {
      digitalWrite(ledG, HIGH);
      digitalWrite(ledR, LOW);
      digitalWrite(ledB, LOW);
    } else if (fsrVoltage >= thres3) {
      digitalWrite(ledB, HIGH);
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, LOW);
    } else {
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, LOW);
    }
    // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
    fsrResistance = 5000 - fsrVoltage;
    fsrResistance *= 10000;
    fsrResistance /= fsrVoltage;

    fsrConductance = 1000000;
    fsrConductance /= fsrResistance;

    // Ffsr = ((Vcc - V) * R) / V
    fsrForce = fsrConductance / 80;
    //Serial.print("Force in Newtons: ");
    //Serial.println(fsrForce);
    Serial.println(fsrVoltage);
  }
  delay(200);
}
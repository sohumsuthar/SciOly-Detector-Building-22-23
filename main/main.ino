int fsrPin = 0;
int ledR = 11;
int ledG = 10;
int ledB = 12;
int resistor = 9720;

float fsrForce;
float fsrVoltage;
float fsrResistance;
float fsrConductance;
float botGreen = 100;
float topGreen = 500;
float botRed = 500;
float topRed = 1000;
float botBlue = 30;
float topBlue = 100;
float e = M_E;

void setup(void) {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  analogReference(EXTERNAL);
}

void loop(void) {
  fsrVoltage = getVoltage() / 1000;
  // Serial.print("mV = ");
  // Serial.println(fsrVoltage);
  // Serial.print(",");

  if (fsrVoltage == 0) {
    Serial.println("No pressure");
  } else {
    // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
    // Ffsr = ((Vcc - V) * R) / V
    fsrForce = getg(fsrVoltage);
    Serial.print("Voltage(V): ");
    Serial.println(fsrVoltage);
    Serial.print("Mass(g): ");
    Serial.println(fsrForce);
    ledstuff(fsrForce);
  }
  delay(200);
}

void ledstuff(float mass) {
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);
  if (mass >= botRed && mass <= topRed) {
    digitalWrite(ledR, HIGH);
  }
  if (mass >= botGreen && mass <= topGreen) {
    digitalWrite(ledG, HIGH);
  }
  if (mass >= botBlue && mass <= topBlue) {
    digitalWrite(ledB, HIGH);
  }
}

float getVoltage() {
  int analogVolts = 0;
  int numSamples = 15;
  for (int i = 0; i < numSamples; i++) {
    analogVolts = analogVolts + analogRead(fsrPin);
    delay(50);
  }
  analogVolts = analogVolts / numSamples;
  return map(analogVolts, 0, 1023, 3300, 00);
}

float getg(float voltage) {
  //return (5.155 * voltage * voltage + 23.07* voltage + 19.09);
  //return (pow(e , (voltage/(-1.604)))/0.001041); //dataset 1
  if (voltage >= 1.00) {                              //mass if voltage is under 1.00V
    return (497.3 * exp(-0.4063 * voltage) - 91.24);  // mass(g)  = (497.3 * (e(-0.4063 * voltage)) - 91.24)
  } else {                                            //mass if voltage is above 1.00V
    return (1713 * exp(-4.09 * voltage) + 235.4);     // mass(g) = (1713 * (e^(-4.09*voltage)) + 235.4)
  }
}
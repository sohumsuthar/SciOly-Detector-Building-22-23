int fsrPin = 0;
int ledR = 7;
int ledG = 4;
int ledB = 2;
int resistor = 1000;
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

void setup(void) {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  analogReference(DEFAULT);
}

void loop(void) {
  fsrVoltage = getVoltage()/1000;
  // Serial.print("mV = ");
  // Serial.println(fsrVoltage);
  // Serial.print(",");


  if (fsrVoltage == 0) {
    Serial.println("No pressure");
  } else {
    // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
  
    // Ffsr = ((Vcc - V) * R) / V
    fsrForce = getNewtons(fsrVoltage);
    Serial.print("Voltage(V): ");
    Serial.println(fsrVoltage);
    Serial.print("Mass(g): ");
    Serial.println(fsrForce);
    ledstuff(fsrForce);
  }
  delay(200);
}
void ledstuff(float mass){
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
float getVoltage(){
  int analogVolts = 0;
  int numSamples = 15;
  for(int i = 0; i<numSamples; i++){
      analogVolts = analogVolts + analogRead(fsrPin);
      delay(50);
  }
  analogVolts = analogVolts/numSamples;
  return map(analogVolts, 0, 1023, 0, 5000);

}
float getNewtons(float voltage){
 return (5.155 * voltage * voltage + 23.07* voltage + 19.09);

}
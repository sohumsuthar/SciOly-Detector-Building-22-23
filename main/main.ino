int fsrPin = 0;
int ledR = 7;
int ledG = 4;
int ledB = 2;
int ledLevel;
float fsrForce;
float fsrVoltage;
float fsrResistance;
float fsrConductance;
float botGreen;
float topGreen;
float botRed;
float topRed;
float botBlue;
float topBlue;

void setup(void) {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  analogReference(DEFAULT);
}

void loop(void) {
  fsrVoltage = getVoltage();
  // Serial.print("mV = ");
  // Serial.println(fsrVoltage);
  // Serial.print(",");


  if (fsrVoltage == 0) {
    Serial.println("No pressure");
  } else {
    // Vout = Vcc * R / (R + FSR) R = 10K, Vcc = 5V
  
    // Ffsr = ((Vcc - V) * R) / V
    fsrForce = getNewtons(fsrVoltage);
    //Serial.print("Force in Newtons: ");
    //Serial.println(fsrForce);
    Serial.println(fsrVoltage);
    Serial.println(fsrForce);
  }
  delay(200);
}
void ledstuff(float volt){
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);
  if (volt >= botRed && volt <= topRed) {
        digitalWrite(ledR, HIGH);
  }
  if (volt >= botGreen && volt <= topGreen) {
        digitalWrite(ledG, HIGH);
  }
  if (volt >= botBlue && volt <= topBlue) {
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
  fsrResistance = 1000 * voltage/(5000-voltage);
  fsrConductance = 1/fsrResistance;
  return fsrConductance*fsrConductance;
}
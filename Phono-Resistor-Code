void setup() {
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if(analogRead(A0) < 700){
    digitalWrite(8, HIGH);
  }
  else{
    digitalWrite(8, LOW);
  }
  Serial.println(analogRead(A0));
}

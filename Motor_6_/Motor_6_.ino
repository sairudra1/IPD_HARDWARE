

#define PUMP_PIN_N 5
#define PUMP_PIN_P 16
#define PUMP_PIN_K 17
#define PUMP_PIN_pH 18
#define PUMP_PIN 19
#define PUMP_PIN2 21

void setup() {

  
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(PUMP_PIN2, OUTPUT);
  pinMode(PUMP_PIN_N, OUTPUT);
  pinMode(PUMP_PIN_P, OUTPUT);
  pinMode(PUMP_PIN_K, OUTPUT);
  pinMode(PUMP_PIN_pH, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(PUMP_PIN2, LOW);
  digitalWrite(PUMP_PIN_N, LOW);
  digitalWrite(PUMP_PIN_P, LOW);
  digitalWrite(PUMP_PIN_K, LOW);
  digitalWrite(PUMP_PIN_pH, LOW);
  delay(9000);
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite(PUMP_PIN2, HIGH);
  digitalWrite(PUMP_PIN_N, HIGH);
  digitalWrite(PUMP_PIN_P, HIGH);
  digitalWrite(PUMP_PIN_K, HIGH);
  digitalWrite(PUMP_PIN_pH, HIGH);
  delay(9000);

  // put your main code here, to run repeatedly:

}

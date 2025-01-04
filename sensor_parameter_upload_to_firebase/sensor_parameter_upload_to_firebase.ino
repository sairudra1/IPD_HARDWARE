#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "Sairudra"
#define WIFI_PASSWORD "12345678"
#include <DHT.h>
int soilMoisture = 0;
int gasValue = 0;
int hum, temp;
bool rainD = false;
const char* ssid = "Sairudra";
const char* password = "12345678";
float temperature = 0;
float humidity = 0;
bool rainDetected = false;
int smoisture = 0;
bool smoke = false;
// Pin configurations
#define DHTPIN 5            // DHT11 data pin
#define DHTTYPE DHT11       // DHT11 sensor type
#define SOIL_SENSOR_PIN 34  // Soil moisture sensor pin (analog)
#define GAS_SENSOR_PIN 35   // MQ2 gas sensor pin (analog)
#define RAIN_SENSOR_PIN 32  // Rain sensor pin (digital)
#define PUMP_PIN 23         // Water pump control pin (relay)

// Sensor objects and variables
DHT dht(DHTPIN, DHTTYPE);
bool pumpState = false;  // Default pump state (OFF)

/* 2. Define the API Key */
#define API_KEY "AIzaSyCAmlESSeubeorQfZrhlo_n2AAyLU6EVj4"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://newyearesp-default-rtdb.firebaseio.com/"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "sairudragudur@gmail.com"
#define USER_PASSWORD "Sairudra@123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin = 4;

void setup() {

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  dht.begin();

  // GPIO setup
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  digitalWrite(PUMP_PIN, LOW);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096, 1024);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}

void sensor() {
  humidity = dht.readHumidity();               //humidity
  temperature = dht.readTemperature();         //temperature
  soilMoisture = analogRead(SOIL_SENSOR_PIN);  //
  gasValue = analogRead(GAS_SENSOR_PIN);
  rainDetected = digitalRead(RAIN_SENSOR_PIN);
  if (gasValue >=  2048) {
    smoke = true;
  } else{
    smoke = false;
  }
  smoisture = map(soilMoisture, 0, 4095, 100, 0);
  if (rainDetected) {
    rainD = false;
  } else {
    rainD = true;  //bollen rail detected or not
  }
}
void loop() {

  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    sensor();
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/temperature", temperature)) {
      Serial.print("temperature level : ");
      Serial.println(temperature);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/humidity", humidity)) {
      Serial.print("humidity level : ");
      Serial.println(humidity);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "sensor/soil_mositure", smoisture)) {
      Serial.print("soil moisture level : ");
      Serial.println(smoisture);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setBool(&fbdo, "sensor/smoke", smoke)) {
      Serial.print("soil moisture level : ");
      Serial.println(smoke);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setBool(&fbdo, "sensor/Rain", rainD)) {
      Serial.print("rain? : ");
      Serial.println(rainD);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }






    /*
    if (Firebase.RTDB.getBool(&fbdo, "/LED/digital")) {
      if (fbdo.dataType() == "boolean") {
        ledStatus = fbdo.boolData();
        Serial.print("LED Status: ");
        Serial.println(ledStatus);
        digitalWrite(LED2_PIN, ledStatus);
      }
    } else {
      Serial.println("FAILED to read LED Status: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "test/int", sensorValue)) {
      Serial.print("pot level : ");
      Serial.println(sensorValue);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Sensor/voltage", voltage)) {
      Serial.print("Voltage: ");
      Serial.println(voltage);
    } else {
      Serial.println("FAILED to save Voltage: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getInt(&fbdo, "/led/state", &ledState)) {
      digitalWrite(ledPin, ledState);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
    */
  }
}

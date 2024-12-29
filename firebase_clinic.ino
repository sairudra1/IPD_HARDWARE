#include <WiFi.h>
#include <Firebase_ESP_Client.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Sairudra"
#define WIFI_PASSWORD "12345678"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCAmlESSeubeorQfZrhlo_n2AAyLU6EVj4"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://newyearesp-default-rtdb.firebaseio.com/"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "sairudragudur@gmail.com"
#define USER_PASSWORD "Sairudra@123"
int sensorPin = 35;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
float voltage = 0;
// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin = 4;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(sensorPin, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}

void loop() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    sensorValue = analogRead(sensorPin);
    
    voltage = (float)sensorValue / 4095 * 3.3; // Calculate voltage (ESP32 ADC is 12-bit)

    int ledState;
    if (Firebase.RTDB.setInt(&fbdo, "test/int", sensorValue)) {
      //  Serial.println("PASSED");
      //  Serial.println("PATH: " + fbdo.dataPath());
      //  Serial.println("TYPE: " + fbdo.dataType());
      Serial.print("pot level : ");
      Serial.println(sensorValue);
    } else {
      Serial.println("FAILED");
      // Serial.println("REASON: " + fbdo.errorReason());
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
  }
}
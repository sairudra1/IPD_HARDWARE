#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#define DHTPIN 4  // GPIO where the DHT11 data pin is connected

// Define the type of DHT sensor
#define DHTTYPE DHT11
#define SOIL_SENSOR_PIN 34  // Soil moisture sensor pin (analog)
#define GAS_SENSOR_PIN 35   // MQ2 gas sensor pin (analog)
#define RAIN_SENSOR_PIN 32  // Rain sensor pin (digital)
#define PUMP_PIN 23         // Water pump control pin (relay)
bool pumpState = false;  // Default pump state (OFF)
bool nitrogenPumpState = false;
bool potassiumPumpState = false;
bool phosphorusPumpState = false;
bool rainDetected = false;
int sm;
#define NITROGEN_PUMP_PIN 25  // Digital pin for nitrogen pump
#define POTASSIUM_PUMP_PIN 26 // Digital pin for potassium pump
#define PHOSPHORUS_PUMP_PIN 27 // Digital pin for phosphorus pump
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Sairudra"
#define WIFI_PASSWORD "12345678"

/* 2. Define the API Key */
//#define API_KEY "AIzaSyCAmlESSeubeorQfZrhlo_n2AAyLU6EVj4"
#define API_KEY "AIzaSyCVHXr_BqNO536tlqrCXo5n01NdbE2zDn4"   //prashant


/* 3. Define the RTDB URL */
//#define DATABASE_URL "https://newyearesp-default-rtdb.firebaseio.com/"
#define DATABASE_URL "https://ipd2024-25-default-rtdb.firebaseio.com/"  //prashant
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "sairudragudur@gmail.com"
#define USER_PASSWORD "Sairudra@123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin = 13;

void setup() {

  pinMode(PUMP_PIN, OUTPUT);

  pinMode(NITROGEN_PUMP_PIN, OUTPUT);
  pinMode(POTASSIUM_PUMP_PIN, OUTPUT);
  pinMode(PHOSPHORUS_PUMP_PIN, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  digitalWrite(PUMP_PIN, LOW);

  digitalWrite(NITROGEN_PUMP_PIN, LOW);
  digitalWrite(POTASSIUM_PUMP_PIN, LOW);
  digitalWrite(PHOSPHORUS_PUMP_PIN, LOW);

  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  dht.begin();
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

  Serial.println("DHT11 Sensor Integration with ESP32");
}

void loop() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    ////////////////sensor////////////////
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Default in Celsius

    int soilMoisture = analogRead(SOIL_SENSOR_PIN);
    int gasValue = analogRead(GAS_SENSOR_PIN);
    int rainDetected = digitalRead(RAIN_SENSOR_PIN);
    
    bool rainD;
    //bool change = !change;
    int smoisture = soilMoisture;
    bool smoke;
    int gas = gasValue;
    if (gas >= 2500) {
      smoke = true;
    }
    else {
      smoke = false;
    }
    if (soilMoisture >= 2000) {
      digitalWrite(PUMP_PIN, HIGH);
      delay(1000);
    }
    else {
      digitalWrite(PUMP_PIN, LOW);
    }
    
    //int barish = random(1000, 4000);
    if (!rainDetected) {
      rainD = true;
    }
    else {
      rainD = false;
    }

    int nitrogen = random(40, 80);
    int pot = random(40, 80);
    int posp = random(40, 80);
    int pH = random(6, 10);


    int ledState;
    int wat, nit, pato, pph;
    /*

      pinMode(PUMP_PIN, OUTPUT);

      pinMode(NITROGEN_PUMP_PIN, OUTPUT);
      pinMode(POTASSIUM_PUMP_PIN, OUTPUT);
      pinMode(PHOSPHORUS_PUMP_PIN, OUTPUT);
    */
    if (Firebase.RTDB.getInt(&fbdo, "/pump/Water", &wat)) {
      digitalWrite(PUMP_PIN, wat);
      Serial.print("water status : ");
      Serial.println(wat);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
    if (Firebase.RTDB.getInt(&fbdo, "/pump/Nitro", &nit)) {
      digitalWrite(NITROGEN_PUMP_PIN, nit);
      Serial.print("nitrogen status : ");
      Serial.println(nit);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
    if (Firebase.RTDB.getInt(&fbdo, "/pump/Pota", &pato)) {
      digitalWrite(POTASSIUM_PUMP_PIN, pato);
      Serial.print("potasium status : ");
      Serial.println(pato);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
    if (Firebase.RTDB.getInt(&fbdo, "/pump/Posph", &pph)) {
      digitalWrite(PHOSPHORUS_PUMP_PIN, pph);
      Serial.print("poshporous status : ");
      Serial.println(pph);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }


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
    if (Firebase.RTDB.setInt(&fbdo, "sensor/soilmositure", smoisture)) {
      Serial.print("soil moisture level : ");
      Serial.println(smoisture);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setBool(&fbdo, "sensor/smoke", smoke)) {
      Serial.print("Smoke detected : ");
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
    ////////////////////////////////////////////////////////////////////////
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/Nitrogen", nitrogen)) {
      Serial.print("nitrogen level : ");
      Serial.println(nitrogen);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/Potassium", pot)) {
      Serial.print("Potassium level : ");
      Serial.println(pot);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/Phosphorus", posp)) {
      Serial.print("Phosphorus level : ");
      Serial.println(temperature);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/pH", pH)) {
      Serial.print("pH level : ");
      Serial.println(pH);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}

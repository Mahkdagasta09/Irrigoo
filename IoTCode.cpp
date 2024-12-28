#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP8266.h>

#define sensorMoisture A0
#define DHTTYPE DHT11
#define sensorDHT D5
DHT dht(sensorDHT, DHTTYPE);
#define Fan D4
#define Pump D6

// Konfigurasi Firebase
#define FIREBASE_HOST "https://gasta-e5b66-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "J2UWFB3dBQnS3j6YECNpiiHK9NNU8F9aTTCCOyzG"

// Inisialisasi LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi Configuration
char ssid[] = "Galaxy A115c66";
char pass[] = "ibvz2940";

// Firebase data object
FirebaseData fbdo;

// Firebase auth and config
FirebaseAuth auth;
FirebaseConfig config;

// Path untuk data Firebase
String path = "DHT11";
String path1 = "Moisture";

// Waktu dan counter
unsigned long dataMilis = 0;

// Fungsi untuk mencetak hasil Firebase
void printResult(FirebaseData &data) {
  Serial.println("Firebase Result:");
  Serial.println("PATH: " + data.dataPath());
  Serial.println("TYPE: " + data.dataType());
  Serial.println("VALUE: " + data.stringData());
}

// Fungsi untuk koneksi Wi-Fi
void connectionWifi() {
  Serial.print("Connecting to Wi-Fi...");
  lcd.setCursor(0, 0);
  lcd.print("Connecting Wi-Fi");
  WiFi.begin(ssid, pass);

  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    delay(1000);
    Serial.print(".");
    retryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("Connected to Wi-Fi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Connected");
    delay(2000);
  } else {
    Serial.println();
    Serial.println("Failed to connect to Wi-Fi.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Failed!");
    while (true) {
      delay(1000);
    }
  }
}

void soilMoisture() {
  int value = analogRead(sensorMoisture);
  value = map(value, 0, 1023, 100, 0);

  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" %");

  if (value < 40) {
    digitalWrite(Pump, LOW);
    Serial.println("Water Pump ON");
  } else {
    digitalWrite(Pump, HIGH);
    Serial.println("Water Pump OFF");
  }
}

void DHTTemperature() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int tempInt = round(temperature);
  int humidityInt = round(humidity);

  lcd.setCursor(0, 1);
  lcd.print("Temperat :");
  lcd.print(tempInt);
  lcd.print(" C");

  if (temperature > 30) {
    digitalWrite(Fan, LOW);
    Serial.println("Fan ON");
  } else {
    digitalWrite(Fan, HIGH);
    Serial.println("Fan OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(Fan, OUTPUT);
  digitalWrite(Fan, HIGH);
  pinMode(Pump, OUTPUT);
  digitalWrite(Pump, HIGH);

  dht.begin();

  // LCD Initiation
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();

  connectionWifi();

  // Konfigurasi Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.reconnectWiFi(true);

  // Inisialisasi Firebase
  Firebase.begin(&config, &auth);
}

void loop() {
  DHTTemperature();
  soilMoisture();
  delay(2000);

  if (millis() - dataMilis > 5000) {
    dataMilis = millis();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilMoistureValue = analogRead(sensorMoisture);
    soilMoistureValue = map(soilMoistureValue, 0, 1023, 100, 0);

    Serial.println("-------------------------------");
    Serial.println("Uploading data to Firebase...");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoistureValue);

    if (Firebase.setFloat(fbdo, path + "/data/kelembapan", humidity)) {
      Serial.println("PASSED - Humidity");
      printResult(fbdo);
    } else {
      Serial.println("FAILED - Humidity");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.setFloat(fbdo, path + "/data/temperature", temperature)) {
      Serial.println("PASSED - Temperature");
      printResult(fbdo);
    } else {
      Serial.println("FAILED - Temperature");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.setInt(fbdo, path1 + "/soil_moisture", soilMoistureValue)) {
      Serial.println("PASSED - Soil Moisture");
      printResult(fbdo);
    } else {
      Serial.println("FAILED - Soil Moisture");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}



// Board: NodeMCU 1.0 (ESP-12E Module)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// === Pin Definitions ===
#define DHTPIN D4        // DHT11 data pin
#define DHTTYPE DHT11
#define MQ2PIN A0        // MQ2 analog pin
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === WiFi Credentials ===
const char* ssid = "Realme C11 2021";
const char* password = "0cdacd5a";

// === ThingSpeak Info ===
const char* server = "http://api.thingspeak.com";
String apiKey = "00OZMPR1VYQSLSSH";
unsigned long cloud_timer = 0;

// === Icons ===
const unsigned char temperatureIcon[] PROGMEM = {
  0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,
  0x03,0x80,0x03,0x80,0x03,0x80,0x07,0xC0,
  0x07,0xC0,0x0F,0xE0,0x0F,0xE0,0x1F,0xF0,
  0x1F,0xF0,0x1F,0xF0,0x1F,0xF0,0x0F,0xE0
};

const unsigned char humidityIcon[] PROGMEM = {
  0x03,0x80,0x07,0xC0,0x0F,0xE0,0x1F,0xF0,
  0x1F,0xF0,0x3F,0xF8,0x3F,0xF8,0x3F,0xF8,
  0x3F,0xF8,0x1F,0xF0,0x0F,0xE0,0x07,0xC0,
  0x03,0x80,0x01,0x00,0x00,0x00,0x00,0x00
};

const unsigned char gasIcon[] PROGMEM = {
  0x0F,0xC0,0x1F,0xE0,0x3F,0xF0,0x3F,0xF0,
  0x7F,0xF8,0x7F,0xF8,0x7F,0xF8,0x7F,0xF8,
  0x7F,0xF8,0x7F,0xF8,0x7F,0xF8,0x3F,0xF0,
  0x3F,0xF0,0x1F,0xE0,0x0F,0xC0,0x00,0x00
};

void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin(D2, D1); // SDA=D2, SCL=D1

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 3);
  display.print("System Started");
  display.display();
  delay(1000);
  display.clearDisplay();

  WiFi.begin(ssid, password);
  display.setCursor(0, 3);
  display.print("Connecting to WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  display.clearDisplay();
  display.setCursor(0, 3);
  display.print("WiFi Connected!");
  display.display();
  delay(1000);
  display.clearDisplay();

  cloud_timer = millis();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ2PIN);
  bool gasDetected = gasValue > 400;

  // If DHT fails, skip this loop
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    display.clearDisplay();
    display.setCursor(0, 3);
    display.print("Sensor Error");
    display.display();
    delay(2000);
    return;
  }

  // === OLED Display ===
  display.clearDisplay();

  display.drawBitmap(0, 5, temperatureIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 5);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.drawBitmap(0, 25, humidityIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");

  display.drawBitmap(0, 45, gasIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 45);
  display.print("Gas: ");
  display.println(gasDetected ? "Detected" : "Not Detected");

  display.display();
  delay(500);
}

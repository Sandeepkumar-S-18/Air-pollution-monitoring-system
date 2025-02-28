// Board ------ NodeMCU 1.0 (ESP-12E Module)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// VCC - 3.3V and GND

// DHT11 sensor setup
#define DHTPIN D4  // D4 on ESP8266
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ2 sensor setup
#define MQ2PIN A0  // Analog pin for MQ2

// OLED display setup
// SCL - D1 and SDA - D2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "**********";  // your hotspot name 
const char* password = "**********";  // your hotspot password 

// ThingSpeak settings
const char* server = "http://api.thingspeak.com";
String apiKey = "00OZMPR1VYQSLSSH";
unsigned long myChannelNumber = 2600368; 

unsigned long cloud_timer = 0;

// Bitmaps for icons (16x16 pixels)
const unsigned char temperatureIcon[] PROGMEM = {
  0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80,
  0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x07, 0xC0,
  0x07, 0xC0, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xF0,
  0x1F, 0xF0, 0x1F, 0xF0, 0x1F, 0xF0, 0x0F, 0xE0
};

const unsigned char humidityIcon[] PROGMEM = {
  0x03, 0x80, 0x07, 0xC0, 0x0F, 0xE0, 0x1F, 0xF0,
  0x1F, 0xF0, 0x3F, 0xF8, 0x3F, 0xF8, 0x3F, 0xF8,
  0x3F, 0xF8, 0x1F, 0xF0, 0x0F, 0xE0, 0x07, 0xC0,
  0x03, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char gasIcon[] PROGMEM = {
  0x0F, 0xC0, 0x1F, 0xE0, 0x3F, 0xF0, 0x3F, 0xF0,
  0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8,
  0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8, 0x3F, 0xF0,
  0x3F, 0xF0, 0x1F, 0xE0, 0x0F, 0xC0, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize OLED display
  Wire.begin(D2, D1); // SDA=D2 (GPIO4), SCL=D1 (GPIO5)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 3);
  display.print("System Started");
  display.display();
  delay(1000);
  display.clearDisplay();

  // Connect to WiFi
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
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas value from MQ2
  int gasValue = analogRead(MQ2PIN);
  bool gasDetected = (gasValue > 400);  // Adjust threshold as needed

  // Display values on OLED screen
  display.clearDisplay();

  // Display temperature icon and value
  display.drawBitmap(0, 5, temperatureIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 5);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  // Display humidity icon and value
  display.drawBitmap(0, 25, humidityIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");

  // Display gas icon and value
  display.drawBitmap(0, 45, gasIcon, 16, 16, SSD1306_WHITE);
  display.setCursor(20, 45);
  display.print("Gas: ");
  if (gasDetected) {
    display.println("Detected");
  } else {
    display.println("Not Detected");
  }

  display.display();

  // Upload to ThingSpeak every 5 seconds
  if ((millis() - cloud_timer) > 5000) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(temperature) + "&field2=" + String(humidity) + "&field3=" + String(gasDetected);
      http.begin(client, url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    cloud_timer = millis();
  }

  delay(500);
}

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

// DHT11 sensor setup
#define DHTPIN 14 // DHT11 data pin connected to GPIO 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ2 sensor setup
#define MQ2PIN 34 // MQ2 analog output pin connected to GPIO 34

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "project";
const char* password = "12345678";

// ThingSpeak settings
const char* server = "api.thingspeak.com";
String apiKey = "00OZMPR1VYQSLSSH";
unsigned long myChannelNumber = 2600368; // Replace with your ThingSpeak channel number
unsigned long cloud_timmer = 0; 


// Bitmaps for icons (16x16 pixels)
const unsigned char temperatureIcon [] PROGMEM = {
  0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80,
  0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x07, 0xC0,
  0x07, 0xC0, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xF0,
  0x1F, 0xF0, 0x1F, 0xF0, 0x1F, 0xF0, 0x0F, 0xE0
};

const unsigned char humidityIcon [] PROGMEM = {
  0x03, 0x80, 0x07, 0xC0, 0x0F, 0xE0, 0x1F, 0xF0,
  0x1F, 0xF0, 0x3F, 0xF8, 0x3F, 0xF8, 0x3F, 0xF8,
  0x3F, 0xF8, 0x1F, 0xF0, 0x0F, 0xE0, 0x07, 0xC0,
  0x03, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char gasIcon [] PROGMEM = {
  0x0F, 0xC0, 0x1F, 0xE0, 0x3F, 0xF0, 0x3F, 0xF0,
  0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8,
  0x7F, 0xF8, 0x7F, 0xF8, 0x7F, 0xF8, 0x3F, 0xF0,
  0x3F, 0xF0, 0x1F, 0xE0, 0x0F, 0xC0, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize MQ2 sensor
  pinMode(MQ2PIN, INPUT);
  Wire.begin(21, 22);
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setTextSize(2);
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");

    display.setCursor(0, 3);
    display.print("Connecting to WiFi");
    display.display();
  }
  Serial.println("Connected to WiFi");
  display.clearDisplay();
  display.setCursor(0, 3);
  display.print("Connected to WiFi");
  display.display();
  display.setTextSize(1);
  cloud_timmer =  millis();
 
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas value from MQ2
  int gasValue = digitalRead(MQ2PIN);
  bool gasDetected = !gasValue; // Adjust threshold as needed

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
  if(gasDetected) {
    display.println("Detected");
  } else {
    display.println("Not Detected");
  }

  display.display();
 
  if ((millis() -  cloud_timmer ) > 5000)
  {
    // Upload values to ThingSpeak
    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String("http://") + server + "/update?api_key=" + apiKey + "&field1=" + temperature + "&field2=" + humidity + "&field3=" + !gasValue;
      http.begin(url.c_str());
      int httpResponseCode = http.GET();
      if(httpResponseCode > 0) {
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
    cloud_timmer = millis();
  }
  delay(500);
   
}

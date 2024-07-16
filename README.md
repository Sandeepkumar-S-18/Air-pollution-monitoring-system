# Air Pollution Monitoring System

This project monitors temperature, humidity, and gas levels using an ESP32 microcontroller with DHT11 and MQ2 sensors. Data is displayed on an OLED screen and uploaded to ThingSpeak for remote monitoring.

#### Features
- Real-time temperature and humidity monitoring.
- OLED display for local data visualization.
- Integration with ThingSpeak for cloud data logging.

#### Components Used
- ESP32 microcontroller
- DHT11 temperature and humidity sensor
- MQ2 gas sensor
- SSD1306 OLED display
- Adafruit SSD1306 and DHT libraries
- WiFi library for ESP32
- ThingSpeak for data logging

#### Setup Instructions
1. **Hardware Setup:**
   - Connect DHT11 to GPIO 14 and MQ2 analog output to GPIO 34.
   - Wire SSD1306 OLED display to ESP32.

2. **Software Setup:**
   - Install required libraries listed in the `#include` section of `main.cpp`.
   - Configure WiFi credentials in `setup()` function.

3. **ThingSpeak Configuration:**
   - Create a ThingSpeak account and channel.
   - Replace `apiKey` and `myChannelNumber` in `main.cpp` with your specific values.

#### Usage
- Power on the device and monitor OLED display for real-time data.
- Data is automatically uploaded to ThingSpeak every 5 seconds.


Feel free to customize this template based on your specific project details and preferences.

#### Author
Sandeepkumar S | sandeep6361460@gmail.com

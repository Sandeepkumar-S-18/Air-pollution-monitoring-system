# Air Pollution Monitoring System

This project monitors temperature, humidity, and gas levels using an ESP32 microcontroller with DHT11 and MQ2 sensors. Data is displayed on an OLED screen and uploaded to ThingSpeak for remote monitoring.

## Features
- Real-time temperature and humidity monitoring.
- OLED display for local data visualization.
- Integration with ThingSpeak for cloud data logging.

## Components Used
- ESP32 microcontroller
- DHT11 temperature and humidity sensor
- MQ2 gas sensor
- SSD1306 OLED display
- Adafruit SSD1306 and DHT libraries
- WiFi library for ESP32
- ThingSpeak for data logging

## Software Requirement:
- [Arduino IDE](https://www.arduino.cc/)
> Arduino IDE used to upload programming in ESP32 Board with required library.
- [Thingspeak](https://thingspeak.com/)
> ThingSpeak is a cloud-based IoT analytics platform service that lets you collect, view, and examine real-time data streams. Data can be sent from your devices to ThingSpeak, enabling instantaneous live data display.

## Setup Instructions
1. **Hardware Setup:**
   - Connect DHT11 to GPIO 14 and MQ2 analog output to GPIO 34.
   - Wire SSD1306 OLED display to ESP32.

2. **Software Setup:**
   - Install required libraries listed in the `#include` section of `main.cpp`.
   - Configure WiFi credentials in `setup()` function.

3. **ThingSpeak Configuration:**
   - Create a ThingSpeak account and channel.
   - Replace `apiKey` and `myChannelNumber` in `main.cpp` with your specific values.

## Usage
- Power on the device and monitor OLED display for real-time data.
- Data is automatically uploaded to ThingSpeak every 5 seconds.

## Result:
<p align="center">
  <img src="Images/ThingSpeak_Result.png"/> 
</p>


## Credit:
- [SANDEEPKUMAR S](https://www.linkedin.com/in/sandeepkumar-s-233721241/)
- [MANOHAR P HIREMATH](https://www.youtube.com/@ManoharHiremath)
- [DARSHAN D](https://www.youtube.com/@Darshan_d_Naik)
- [RAGHAVENDRA M](https://www.instagram.com/_raghu_m_.46?igsh=MXNjNGcydG1vaWhiOQ==)

# school-bell-system
ESP32-based school bell system with Web application and Windows desktop application

School Bell System is a smart school bell solution based on an ESP32 microcontroller.  
The system controls a physical school bell via relay, displays information on an LED matrix,  
and allows wireless configuration over WiFi using a web application or a Windows desktop application.

The preferred way of controlling the system is through the built-in Web Application hosted on ESP32,
while the Windows application is still available for compatibility.

---

## System Overview

The system consists of four main parts:

- **ESP32 firmware** – controls the bell, LED matrix, web server and stores schedules
- **Web application (HTML / CSS / JavaScript)** – main interface for configuration (runs in browser)
- **Windows desktop application (C#)** – alternative interface for configuration
- **Hardware components** – ESP32, relay module, LED matrix panel

Communication with ESP32 is done over **WiFi using HTTP requests and JSON protocol**.

The ESP32 runs its own web server, so the system can be controlled from any device
(phone, laptop, tablet) without installing software.

---

## Features

- Wireless configuration over WiFi
- Built-in Web Interface (ESP32 Web Server)
- JSON-based communication protocol
- Configurable school bell schedule
- LED matrix notifications for students
- Relay-based bell control
- Real-time updates
- Non-volatile memory storage (data preserved after power loss)
- Manual bell testing
- Works from phone, PC or tablet via browser
- Windows desktop application support

---

## Web Application

The new version includes a full web application written in:

- HTML
- CSS
- JavaScript

The web app is stored in ESP32 flash (LittleFS) and loaded in the browser.

It communicates with ESP32 using:

- HTTP requests
- JSON messages

Advantages of web version:

- No installation required
- Works on any operating system
- Works on mobile devices
- Faster configuration
- Easier maintenance

---

## Windows Desktop Application

The project also includes a Windows desktop application written in C# (Windows Forms).

This version allows:

- Schedule editing
- Message sending
- Manual bell control

This version is kept for compatibility,
but the Web Application is recommended.

---

## Hardware

- ESP32
- Relay module (for bell control)
- HUB75 LED matrix panel
- Power supply
- School bell / buzzer

---

## Technologies Used

- ESP32 (Arduino IDE)
- HTML / CSS / JavaScript
- ESP32 Web Server
- HTTP / JSON communication
- LittleFS file system
- C# Windows Forms (.NET)
- GitHub for version control

---

## Communication Protocol

All data between client and ESP32 is sent using JSON.

Example:

```json
{
  "command": "setSchedule",
  "day": "Monday",
  "time": "08:00"
}

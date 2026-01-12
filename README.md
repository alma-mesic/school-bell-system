# school-bell-system
ESP32-based school bell system with Windows desktop application

School Bell System is a smart school bell solution based on an ESP32 microcontroller.
The system controls a physical school bell via relay and displays information on an LED matrix.
All settings and schedules are managed through a Windows desktop application written in C#.

---

## System Overview

The system consists of three main parts:
- **ESP32 firmware** – controls the bell, LED matrix and stores schedules
- **Windows desktop application** – user-friendly interface for managing schedules and messages
- **Hardware components** – ESP32, relay module, LED matrix

Communication between the desktop application and ESP32 is achieved using **WiFi (HTTP + JSON)**.

---

## Features

- Configurable school bell schedule
- LED matrix notifications for students
- Relay-based bell control
- Real-time communication over WiFi
- Non-volatile memory storage (data preserved after power loss)
- Manual bell testing
- User-friendly Windows interface

---

## Technologies Used

- **ESP32** (Arduino IDE)
- **C# Windows Forms (.NET)**
- **HTTP / JSON communication**
- **GitHub** for version control and collaboration

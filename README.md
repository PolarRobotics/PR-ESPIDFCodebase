# Polar Robotics - ESP-IDF Codebase

Welcome to the official repository for the **Ohio Northern University (ONU) Polar Robotics** team! This codebase provides the firmware for our robotic football players, competing in the **Collegiate Robotic Football Conference (CRFC)**.

This project is built using the **Espressif IoT Development Framework (ESP-IDF)** and is designed to run on ESP32 microcontrollers. 

## 🏈 Overview

The framework is organized into dedicated components that handle various systems in a football robot, ensuring modularity, easy maintenance, and high performance on the field. 

Key capabilities include:
- **Motor & Drive Control:** Interface for diverse motor drivers including PWM and Serial-based drivers (e.g., USB Sabertooth).
- **Controller Integration:** Real-time PS5 controller support via Bluetooth.
- **Smart Pairing:** Easy bot-pairing mechanisms using ESP RainMaker / BLE.
- **Diagnostics & Telemetry:** Built on ESP-Insights and ESP-Diagnostics.

## 📂 Repository Structure

The project is structured following the standard ESP-IDF component-based architecture:

- `main/` - The entry point of the application. Contains specific execution targets like `main_robot.cpp` and `main_write_bot_info.cpp`.
- `components/` - Core internal libraries and modules:
  - `Drive/` - High-level drivetrain logic and kinematics.
  - `Motor/` - Low-level motor control (`PWMMotor.cpp`, `SerialMotor.cpp`).
  - `Pairing/` - Connection and pairing logic for controllers and field management.
  - `Robot/` - Robot-specific state machines and operational profiles.
  - `Utilities/` & `Types/` - Helper functions and common data structures.

## 🚀 Getting Started

### Prerequisites

This project utilizes a Docker-based VS Code Dev Container to provide a fully configured ESP-IDF environment out of the box. 

1. **Docker:** Ensure [Docker](https://www.docker.com/get-started) is installed and running.
2. **Visual Studio Code:** Install [VS Code](https://code.visualstudio.com/).
3. **VS Code Extensions:** Install the **Dev Containers** (`ms-vscode-remote.remote-containers`) extension.
4. **Git:** Any suitable git client. If you are uncertain, just use [GitHub Desktop!](https://desktop.github.com/download/)

Once cloned, simply open the project in VS Code and click **"Reopen in Container"** when prompted.

## 📜 License

This project is licensed under the **GNU Affero General Public License v3.0 or later (AGPL-3.0-or-later)**. 

By contributing to this project, you agree that your contributions will be licensed under its AGPL-3.0-or-later license. See the `LICENSE` file for full details.

We use this license with the intention of fostering collaboration and education within the CRFC. By keeping the codebase open source, and other codebases that build upon it, we keep the environment focused on these goals, as well as minimizing programming gaps between teams.

---
*Developed with 🐻‍❄️ pride by the ONU Polar Robotics Team.*
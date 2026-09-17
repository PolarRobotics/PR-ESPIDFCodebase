# ESP32
## NodeMCU-32s ESP32-S1-DevKitC-1
- [ESP-datasheet](../_static/files/hardware/esp/esp32-solo-1-datasheet-en.pdf)

### Pin Layout
- [ESP Pin Reference Spreadsheet](https://docs.google.com/spreadsheets/d/17pdff4T_3GTAkoctwm2IMg07Znoo-iJkyDGN5CqXq3w/edit#gid=0)

![ESP-pinout](../_static/images/hardware/esp/esp32-pinout.png)


### CPU and Internal Memory
- The ESP32-S0WD contains one low-power Xtensa® 32-bit LX6 microprocessor. The internal memory includes:
  - 448 KB of ROM for booting and core functions.
  - 520 KB of on-chip SRAM for data and instructions.
  - 8 KB of SRAM in RTC, which is called RTC FAST Memory and can be used for data storage; it is accessed by the main CPU during RTC Boot from the deep-sleep mode.
  - 8 KB of SRAM in RTC, which is called RTC SLOW Memory and can be accessed by the co-processor during the Deep-sleep mode.
  - 1 Kib of eFuse, of which:
    - 256 bits are used for the system (MAC address and chip configuration) 
    - 768 bits are reserved for customer applications, including flash-encryption and chip-ID.

### Absolute Maximum Ratings 
| Symbol | Parameter            | Min  | Max | Unit |
| ------ | -------------------- | ---- | --- | ---- |
| VDD3   | Power Supply Voltage | -0.3 | 3.6 | V    |

% TODO: finish AMRs

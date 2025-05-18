# Snake Game on ESP32 with TFT Display

A simple Snake game built with an ESP32 microcontroller, a TFT display, and a joystick module.

## Components Used

- **ESP-WROOM-32** (ESP32 Dev Board)
- **2.4” TFT Display** (240x320, ILI9341 controller)
- **KY-023 Joystick** (X, Y analog and button input)
  
## Wiring Diagram

![image](https://github.com/user-attachments/assets/b011b316-af43-47ab-ba92-aa1893ed2dd0)

### TFT 2.4” Display to ESP32

| TFT Pin     | ESP32 GPIO | 
|-------------|------------|
| `SCK`       | GPIO 18    |
| `SDI / MOSI`| GPIO 23    |
| `DC`        | GPIO 16    |
| `RESET`     | GPIO 17    |
| `CS`        | GPIO 5     |
| `LED, VCC`  | 3.3V       |
| `GND`       | GND        |

###  Joystick KY-023 to ESP32

| Joystick Pin | ESP32 GPIO |
|--------------|------------|
| `VRx`        | GPIO 34    |
| `VRy`        | GPIO 35    |
| `SW`         | GPIO 25    | 
| `VCC`        | 3.3V       |
| `GND`        | GND        |

## Required Libraries
- **[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)**  – a library for controlling TFT displays such as the ILI9341.

## How to Run

1. Upload the code to your ESP32 using the Arduino IDE.
2. Connect all components as shown in the wiring diagram.
3. Choose a speed level from the menu and start playing!

## Controls

- Move the snake using the joystick
- Press the joystick to confirm menu selections

## Gameplay Preview 

<p align="center">
  <img src="https://github.com/user-attachments/assets/ce1fd713-5ba5-47af-9c2e-04478bf367cc" width="45%" />
  <img src="https://github.com/user-attachments/assets/62da6d8b-ff2a-428c-90a9-5c6b50ac323f" width="45%" />
</p>


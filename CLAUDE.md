# CLAUDE.md

## Project Overview

This is an Arduino-based mass detector built for the **Science Olympiad "Detector Building" event (2022-2023 season)**. The device uses a **Force Sensing Resistor (FSR)** to measure the mass of objects placed on it, converts the FSR voltage reading into a mass estimate (in grams) using empirically-derived curve-fit equations, and indicates the mass range via an RGB LED.

## Tech Stack

- **Language:** C++ (Arduino dialect)
- **Platform:** Arduino (ATmega-based, likely Arduino Uno)
- **IDE:** Arduino IDE (`.ino` files), with some Theia/VS Code debug config present
- **External Library:** `Oversampling.h` (used in `main/main.ino` for ADC oversampling)
- **Data Collection Tool:** Vernier Graphical Analysis (`.gambl` files for curve fitting)

## Directory Structure

```
.
├── main/
│   ├── main.ino              # Primary Arduino sketch (uses Oversampling library)
│   └── .theia/
│       └── launch.json       # Debug configuration (cortex-debug, OpenOCD/JLink)
├── STATE/
│   └── STATE.ino             # Alternate/competition-tuned sketch (no Oversampling library)
├── linearizations/
│   ├── dataset1scioly.gambl  # Vernier Graphical Analysis curve-fit data (mass vs voltage)
│   ├── State2.gambl          # State competition calibration data (voltage vs mass)
│   ├── state 3v-1.04 .gambl  # Additional state calibration dataset
│   └── HAHAHAHA STATE LINEAR MOMENT.gambl  # Another calibration dataset
├── detectorimage.jpg         # Photo of the physical detector hardware
├── .gitattributes            # Git line-ending normalization
└── .DS_Store                 # macOS metadata (can be ignored)
```

## Key Files

### `main/main.ino`
- The main sketch used for general competition. Uses the `Oversampling` library for more accurate ADC readings.
- Reads FSR on analog pin 0, converts voltage to mass using piecewise exponential curve fits.
- RGB LED on digital pins 10 (green), 11 (red), 12 (blue) indicates mass range:
  - Green: 0-100g
  - Blue: 101-140g
  - Red: 141-1000g
- Uses external analog reference (`analogReference(EXTERNAL)`) at 3.3V.
- Serial output at 19200 baud for debugging (prints voltage and mass).
- ADC oversampling: 10-bit to 15-bit, factor of 2.

### `STATE/STATE.ino`
- A variant tuned for state competition with different calibration curves and LED thresholds.
- Does NOT use the `Oversampling` library; uses raw `analogRead()` with averaging (20 samples, 50ms delay each).
- Different LED thresholds:
  - Blue: 30-520g
  - Green: 100-450g (also lights red LED)
  - Red: 470-1000g
- Uses a different piecewise curve fit with three voltage ranges and a linear+offset model for the mid-range.

### `linearizations/*.gambl`
- Vernier Graphical Analysis files containing calibration datasets.
- These map FSR voltage readings to known masses for curve fitting.
- The curve-fit coefficients from these files are used in the `getg()` functions in the `.ino` sketches.

## Hardware Setup

- **Sensor:** Force Sensing Resistor (FSR) with a 3D-printed case
- **Reference Resistor:** ~9720 ohm in voltage divider with the FSR
- **Analog Reference:** External 3.3V reference (connected to Arduino AREF pin)
- **LED Pins:** Red=11, Green=10, Blue=12 (digital output)
- **FSR Pin:** Analog pin 0
- **Voltage Divider:** Vout = Vcc * R / (R + FSR), mapped from ADC range (0-1023) to 0-3300mV

## Build and Upload

1. **Install Arduino IDE** (or use Arduino CLI).
2. **Install the `Oversampling` library** (required by `main/main.ino`):
   - In Arduino IDE: Sketch > Include Library > Manage Libraries > search "Oversampling"
3. **Open the sketch:**
   - For general use: open `main/main.ino`
   - For state competition calibration: open `STATE/STATE.ino`
4. **Select board:** Arduino Uno (or whichever ATmega board is used).
5. **Upload** to the Arduino via USB.
6. **Serial Monitor:** Set baud rate to 19200 to view voltage and mass readings.

There is no automated build system, test suite, or CI/CD pipeline. Compilation and upload are done through the Arduino IDE.

## Coding Conventions

- Arduino `.ino` sketch format (C++ with `setup()` and `loop()` entry points)
- Global variables declared at the top of the file
- Helper functions (`getVoltage()`, `getg()`, `ledstuff()`) defined after `loop()`
- Commented-out code is preserved for alternative calibration equations
- Variable naming: camelCase (e.g., `fsrVoltage`, `fsrForce`, `botGreen`)
- Pin assignments as global `int` variables
- Serial baud rate: 19200

## Important Notes

- The `getg()` function uses **piecewise curve-fit equations** derived from physical calibration data. These equations are specific to the particular FSR and resistor used. Changing hardware requires recalibration.
- The `map()` call in `getVoltage()` maps ADC values (0-1023) to millivolts (3300-0), note the reversed order: higher ADC values map to lower voltages because of the voltage divider configuration.
- The `STATE/STATE.ino` variant has different calibration curves optimized for a specific competition setup. The two sketches are not interchangeable without hardware adjustments.
- The `.gambl` files are binary/XML hybrid files from Vernier Graphical Analysis software and are used for reference only; they are not consumed by the Arduino code.

# MiniOS-GUI-Simulator (SFML)
<img src =" resources/assets/os-cover.jpg">
A graphical Mini Operating System Simulator written in C++ using **SFML**.
This project visualizes processes and memory allocation, and supports simple FCFS and Round-Robin simulation runs.

## Features (v1.0)
- Process creation (randomized values) via GUI button
- Terminate last created process
- FCFS and Round-Robin scheduling (RR uses a fixed quantum)
- Memory visualization using First-Fit allocation and simple merging
- Simple, lightweight SFML-based UI (buttons + visual panels)

## Requirements
- C++17 compatible compiler (g++, clang++)
- SFML 2.5 or later installed (system packages or from https://www.sfml-dev.org)
- A TTF font file placed at `resources/fonts/DejaVuSans.ttf` (or change `FONT_PATH` in code)

On Debian/Ubuntu you can install SFML system-wide:
```bash
sudo apt update
sudo apt install libsfml-dev
```

## Build (CMake)
```bash
git clone https://github.com/kiprutobeauttah/MiniOs_Simulator.git
git push -u origin main
cd MiniOS-GUI-Simulator
mkdir build && cd build
cmake ..
make
./MiniOS-GUI-Simulator
```

Or use the provided Makefile if SFML is available in your compiler's include/link paths:
```bash
make
./MiniOS-GUI-Simulator
```

## Notes
- The UI currently generates processes with randomized names and values when you click **Create Process**. This keeps the demo self-contained.
- Place a TTF font at `resources/fonts/DejaVuSans.ttf`. You can download DejaVu Sans or any other TrueType font and rename it accordingly.
- Future improvements: form-based process creation, animated scheduler stepping, process inspector, drag/drop memory, saving/loading scenarios, and unit tests.

---
| powered by Beauttah


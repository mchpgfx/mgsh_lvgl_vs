# PIC32MZ DA LVGL Demo

LVGL graphics demo project for the Microchip PIC32MZ DA microcontroller running on the Curiosity development board with a WVGA (800x480) display.

## Overview

This project demonstrates the integration of [LVGL](https://lvgl.io/) (Light and Versatile Graphics Library) v9.5.0 with the MPLAB Harmony v3 framework on the PIC32MZ DA MCU. It uses the internal GLCD (Graphics LCD Controller) with DDR memory for double-buffered direct rendering and the MaxTouch driver for capacitive touch input.

## Hardware Requirements

- **MCU**: PIC32MZ2064DAR176 (MIPS32 M-Class core with integrated graphics)
- **Board**: [PIC32MZ DA Curiosity Development Board](https://www.microchip.com/en-us/development-tool/EV87D54A)
- **Display**: [High-Performance WVGA Display Module with maXTouch](https://www.microchip.com/en-us/development-tool/AC320005-5)
- **Debugger/Programmer**: ICD 4/5 or compatible

## Software Requirements

- **IDE**: MPLAB X IDE or VS Code with MPLAB Extension
- **Toolchain**: XC32 Compiler v5.00 or later
- **Framework**: MPLAB Harmony v3
- **Device Packs**:
  - PIC32MZ-DA_DFP

## Features

- LVGL v9.5.0 integration with direct rendering mode
- 16-bit RGB565 color depth
- 128KB LVGL memory pool
- Double-buffered display using DDR memory
- Touch input support via MaxTouch I2C driver
- Performance monitor overlay (FPS/CPU usage)
- Hardware 2D GPU acceleration (Nano2D)
- Multiple demo applications:
  - Widgets demo
  - Benchmark demo
  - Stress test demo
  - Music player demo
  - Flex layout demo

## Project Structure

| Path | Description |
|------|-------------|
| `curiosity_wvga/` | Main application source |
| `curiosity_wvga/src/main.c` | Application entry point |
| `curiosity_wvga/src/app_lvgl.c` | LVGL integration and display driver |
| `curiosity_wvga/src/lv_conf.h` | LVGL configuration |
| `curiosity_wvga/src/config/default/` | Harmony generated configuration |
| `curiosity_wvga/src/config/default/gfx/` | Graphics driver (GLCD, Nano2D) |
| `_build/` | CMake build tree (can be deleted) |
| `cmake/` | Generated CMake files |
| `out/` | Final build artifacts (.elf, .hex) |
| `.vscode/` | VS Code and MPLAB project files |

## Building

### Using VS Code with MPLAB Extension

1. Open the project folder in VS Code
2. The MPLAB extension will detect the project from `.vscode/mgsh_pic32mzda_lvgl.mplab.json`
3. Select the `default` configuration
4. Build using the MPLAB extension build command

### Using CMake

```bash
cd _build/mgsh_pic32mzda_lvgl/default
cmake --build .
```

## Configuration

### Display Settings

Edit `curiosity_wvga/src/app_lvgl.c`:

```c
#define DISP_WIDTH 800
#define DISP_HEIGHT 480
```

### LVGL Settings

Edit `curiosity_wvga/src/lv_conf.h`:

```c
#define LV_COLOR_DEPTH 16           // RGB565
#define LV_MEM_SIZE (128 * 1024U)   // 128KB heap
#define LV_DEF_REFR_PERIOD 33       // ~30 FPS
#define LV_DPI_DEF 130              // Dots per inch
```

### Selecting a Demo

In `lv_conf.h`, enable one of the following demos:

```c
#define LV_USE_DEMO_WIDGETS 1
#define LV_USE_DEMO_BENCHMARK 0
#define LV_USE_DEMO_STRESS 0
#define LV_USE_DEMO_MUSIC 0
#define LV_USE_DEMO_FLEX_LAYOUT 0
```

## Architecture

```
main()
  └─> SYS_Initialize()    // Initialize Harmony modules
  └─> SYS_Tasks()         // Main loop
        ├─> APP_LVGL_Tasks()   // LVGL state machine
        │     ├─> lv_init()
        │     ├─> lv_display_create()
        │     ├─> lv_indev_create()
        │     └─> lv_timer_handler()
        └─> DRV_MAXTOUCH_Tasks()  // Touch driver
```

## Peripherals Used

| Peripheral | Function |
|------------|----------|
| GLCD | Internal Graphics LCD Controller |
| DDR | External DDR memory for frame buffers |
| Nano2D | Hardware 2D GPU acceleration |
| I2C2 | I2C for MaxTouch controller |
| Core Timer | System time and LVGL ticks |
| GPIO | Display control signals |

## License

This project uses code from:
- Microchip Technology Inc. (Harmony framework, drivers)
- LVGL LLC (LVGL graphics library - MIT License)

## Resources

- [LVGL Documentation](https://docs.lvgl.io/)
- [PIC32MZ DA Product Page](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/32-bit-mcus/pic32-32-bit-mcus/pic32mz-da)
- [MPLAB Harmony v3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony)

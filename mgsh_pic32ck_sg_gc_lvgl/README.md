# PIC32CK SG/GC LVGL Demo

LVGL graphics demo project for the Microchip PIC32CK SG/GC microcontroller running on the Curiosity Ultra development board with a WQVGA (480x272) display.

## Overview

This project demonstrates the integration of [LVGL](https://lvgl.io/) (Light and Versatile Graphics Library) v9.5.0 with the MPLAB Harmony v3 framework on the PIC32CK GC01 MCU. It uses the Low-Cost Controllerless (LCC) display driver for graphics output and the MaxTouch driver for capacitive touch input.

## Hardware Requirements

- **MCU**: PIC32CK2051GC01144 (Cortex-M33 core)
- **Board**: [PIC32CK SG/GC Curiosity Ultra Development Board](https://www.microchip.com/en-us/development-tool/EV71N66A)
- **Display**: [High-Performance 4.3" WQVGA Display Module with maXTouch](https://www.microchip.com/en-us/development-tool/ac320005-4)
- **Adapter**: [565 LCD Adapter Graphics Card](https://www.microchip.com/en-us/development-tool/ac320212)
- **Debugger/Programmer**: ICD 5 or compatible

## Software Requirements

- **IDE**: MPLAB X IDE or VS Code with MPLAB Extension
- **Toolchain**: XC32 Compiler v5.00 or later
- **Framework**: MPLAB Harmony v3
- **Device Packs**:
  - PIC32CK-GC_DFP v1.2.168
  - CMSIS v6.3.0

## Features

- LVGL v9.5.0 integration with partial rendering mode
- 16-bit RGB565 color depth
- 64KB LVGL memory pool
- 32KB render buffer (uncached for DMA compatibility)
- Touch input support via MaxTouch I2C driver
- Performance monitor overlay (FPS/CPU usage)
- Multiple demo applications:
  - Widgets demo
  - Benchmark demo
  - Stress test demo
  - Music player demo
  - Flex layout demo

## Project Structure

| Path | Description |
|------|-------------|
| `curiosity_wqvga_lcc/` | Main application source |
| `curiosity_wqvga_lcc/src/main.c` | Application entry point |
| `curiosity_wqvga_lcc/src/app_lvgl.c` | LVGL integration and display driver |
| `curiosity_wqvga_lcc/src/lv_conf.h` | LVGL configuration |
| `curiosity_wqvga_lcc/src/config/default/` | Harmony generated configuration |
| `curiosity_wqvga_lcc/src/packs/` | Device and CMSIS headers |
| `_build/` | CMake build tree (can be deleted) |
| `cmake/` | Generated CMake files |
| `out/` | Final build artifacts (.elf, .hex) |
| `.vscode/` | VS Code and MPLAB project files |

## Building

### Using VS Code with MPLAB Extension

1. Open the project folder in VS Code
2. The MPLAB extension will detect the project from `.vscode/mgsh_pic32ck_sg_gc_lvgl.mplab.json`
3. Select the `default` configuration
4. Build using the MPLAB extension build command

### Using CMake

```bash
cd _build/mgsh_pic32ck_sg_gc_lvgl/default
cmake --build .
```

## Configuration

### Display Settings

Edit `curiosity_wqvga_lcc/src/app_lvgl.c`:

```c
#define DISP_WIDTH 480
#define DISP_HEIGHT 272
#define LV_RENDER_BUFFER_SIZE (32 * 1024)
```

### LVGL Settings

Edit `curiosity_wqvga_lcc/src/lv_conf.h`:

```c
#define LV_COLOR_DEPTH 16           // RGB565
#define LV_MEM_SIZE (64 * 1024U)    // 64KB heap
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
| EBI | External Bus Interface for display memory |
| DMA0 | DMA for display buffer transfers |
| TCC0/TCC1/TCC5 | Timers for system time and LVGL ticks |
| SERCOM4 | I2C for MaxTouch controller |
| PORT | GPIO for display control signals |

## License

This project uses code from:
- Microchip Technology Inc. (Harmony framework, drivers)
- LVGL LLC (LVGL graphics library - MIT License)

## Resources

- [LVGL Documentation](https://docs.lvgl.io/)
- [PIC32CK GC Product Page](https://www.microchip.com/en-us/product/PIC32CK2051GC01144)
- [MPLAB Harmony v3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony)

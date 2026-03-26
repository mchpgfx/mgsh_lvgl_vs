# SAM9X60 LVGL Demo

LVGL graphics demo project for the Microchip SAM9X60 microprocessor running on the Curiosity development board with a WVGA (800x480) display.

## Overview

This project demonstrates the integration of [LVGL](https://lvgl.io/) (Light and Versatile Graphics Library) v9.5.0 with the MPLAB Harmony v3 framework on the SAM9X60 MPU. It uses the built-in LCD Controller (LCDC) with 2D Graphics Processor (GFX2D) for hardware-accelerated graphics output and the MaxTouch driver for capacitive touch input.

## Hardware Requirements

- **MCU**: SAM9X60D1G (ARM926EJ-S core)
- **Board**: [SAM9X60 Curiosity Development Board](https://www.microchip.com/en-us/development-tool/ev40e67a)
- **Display**: [High-Performance WVGA Display Module with maXTouch](https://www.microchip.com/en-us/development-tool/ac320005-5)
- **Debugger/Programmer**: PICkit 5 or compatible

## Software Requirements

- **IDE**: MPLAB X IDE or VS Code with MPLAB Extension
- **Toolchain**: XC32 Compiler v5.00 or later
- **Framework**: MPLAB Harmony v3
- **Device Packs**:
  - SAM9X6_DFP v1.15.272

## Features

- LVGL v9.5.0 integration with partial rendering mode
- 16-bit RGB565 color depth
- 512KB LVGL memory pool
- Full frame buffer in uncached DDR memory for DMA compatibility
- Hardware-accelerated graphics via GFX2D
- Touch input support via MaxTouch I2C driver
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
| `curiosity_wvga/src/packs/` | Device headers |
| `binaries/` | AT91Bootstrap bootloader |
| `_build/` | CMake build tree (can be deleted) |
| `cmake/` | Generated CMake files |
| `out/` | Final build artifacts (.elf, .bin) |
| `.vscode/` | VS Code and MPLAB project files |

## Building

### Using VS Code with MPLAB Extension

1. Open the project folder in VS Code
2. The MPLAB extension will detect the project from `.vscode/mgsh_sam9x60_lvgl.mplab.json`
3. Select the `default` configuration
4. Build using the MPLAB extension build command

### Using CMake

```bash
cd _build/mgsh_sam9x60_lvgl/default
cmake --build .
```

## Configuration

### Display Settings

Edit `curiosity_wvga/src/app_lvgl.c`:

```c
#define DISP_WIDTH 800
#define DISP_HEIGHT 480
#define APP_USE_DIRECT_RENDER_MODE 0  // 0: partial, 1: direct
```

### LVGL Settings

Edit `curiosity_wvga/src/lv_conf.h`:

```c
#define LV_COLOR_DEPTH 16              // RGB565
#define LV_MEM_SIZE (512 * 1024U)      // 512KB heap
#define LV_DEF_REFR_PERIOD 33          // ~30 FPS
#define LV_DPI_DEF 130                 // Dots per inch
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
  +-> SYS_Initialize()    // Initialize Harmony modules
  +-> SYS_Tasks()         // Main loop
        +-> APP_LVGL_Tasks()   // LVGL state machine
        |     +-> lv_init()
        |     +-> lv_display_create()
        |     +-> lv_indev_create()
        |     +-> lv_timer_handler()
        +-> DRV_MAXTOUCH_Tasks()  // Touch driver
```

## Peripherals Used

| Peripheral | Function |
|------------|----------|
| LCDC | LCD Controller for display output |
| GFX2D | 2D Graphics hardware acceleration |
| TC0 | Timer for system time and LVGL ticks |
| FLEXCOM6 | I2C for MaxTouch controller |
| PIO | GPIO for display control signals |
| AIC | Advanced Interrupt Controller |
| MMU | Memory Management Unit |

## Boot Process

This project uses AT91Bootstrap as the second-stage bootloader to initialize DDR memory and load the application. The bootloader binary is located in `binaries/at91bootstrap.elf`.

## License

This project uses code from:
- Microchip Technology Inc. (Harmony framework, drivers)
- LVGL LLC (LVGL graphics library - MIT License)

## Resources

- [LVGL Documentation](https://docs.lvgl.io/)
- [SAM9X60 Product Page](https://www.microchip.com/en-us/product/SAM9X60)
- [MPLAB Harmony v3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony)

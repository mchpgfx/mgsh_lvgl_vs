# SAM9X75 LVGL Demo

LVGL graphics demo project for the Microchip SAM9X75 microprocessor running on the Curiosity development board with a WVGA (800x480) display.

## Overview

This project demonstrates the integration of [LVGL](https://lvgl.io/) (Light and Versatile Graphics Library) v9.5.0 with the MPLAB Harmony v3 framework on the SAM9X75 MPU. It uses the XLCDC (eXtended LCD Controller) display driver for graphics output and the Input System Service with MaxTouch driver for capacitive touch input.

## Hardware Requirements

- **MCU**: SAM9X75D2G (ARM926EJ-S core)
- **Board**: [SAM9X75 Curiosity Development Board](https://www.microchip.com/en-us/development-tool/EV31H43A) (EV31H43A)
- **Display**: [5" LVDS WVGA 800x480 maXTouch Display Module](https://www.microchipdirect.com/product/AC69T88A) (AC69T88A)
- **Power**: Micro USB cable or 5V power adapter
- **Storage**: microSD card (for boot firmware, inserted into SD BOOT slot)
- **Debugger/Programmer**: J-Link or compatible

For detailed hardware setup instructions, see the [Hardware Setup Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/dev-kits/sam9x75-ug/ac69t88a/).

## Software Requirements

- **IDE**: MPLAB X IDE or VS Code with MPLAB Extension
- **Toolchain**: XC32 Compiler v5.00 or later
- **Framework**: MPLAB Harmony v3
- **Device Packs**:
  - SAM9X7_DFP v1.13.225

## Features

- LVGL v9.5.0 integration with direct rendering mode
- 16-bit RGB565 color depth
- 512KB LVGL memory pool
- Full-screen render buffer (uncached for DMA compatibility)
- Touch input support via MaxTouch I2C driver
- AT91Bootstrap for DDR initialization and boot
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
| `binaries/` | AT91Bootstrap binary for boot |
| `_build/` | CMake build tree (can be deleted) |
| `cmake/` | Generated CMake files |
| `out/` | Final build artifacts (.elf, .hex) |
| `.vscode/` | VS Code and MPLAB project files |

## Building

### Using VS Code with MPLAB Extension

1. Open the project folder in VS Code
2. The MPLAB extension will detect the project from `.vscode/mgsh_sam9x75_lvgl.mplab.json`
3. Select the `default` configuration
4. Build using the MPLAB extension build command

### Using CMake

```bash
cd _build/mgsh_sam9x75_lvgl/default
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
#define LV_MEM_SIZE (512 * 1024U)   // 512KB heap
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
        └─> Touch Driver Tasks  // Touch input processing
```

## Peripherals Used

| Peripheral | Function |
|------------|----------|
| XLCDC | LCD controller for display output |
| XDMAC | DMA for display buffer transfers |
| PIT/TC | Timer for system time and LVGL ticks |
| FLEXCOM (TWI) | I2C for MaxTouch controller |
| PIO | GPIO for display control signals |
| DDR | External DDR memory for framebuffer |

## Boot Process

This project requires AT91Bootstrap to initialize the DDR memory and load the application. The bootstrap binary is located in the `binaries/` folder and is automatically loaded by the debugger.

## License

This project uses code from:
- Microchip Technology Inc. (Harmony framework, drivers)
- LVGL LLC (LVGL graphics library - MIT License)

## Resources

- [LVGL Documentation](https://docs.lvgl.io/)
- [SAM9X75 Product Page](https://www.microchip.com/en-us/product/SAM9X75)
- [MPLAB Harmony v3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony)

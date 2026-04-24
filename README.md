# MPLAB Harmony LVGL Projects

A collection of [LVGL](https://lvgl.io/) (Light and Versatile Graphics Library) demo projects for Microchip microcontrollers and microprocessors using the MPLAB Harmony v3 framework.

## Overview

This repository provides ready-to-use LVGL v9.5.0 graphics demo projects for various Microchip development boards. Each project demonstrates the integration of LVGL with MPLAB Harmony drivers for display output and capacitive touch input.

## Supported Platforms

| Project | MCU/MPU | Core | Display | Board |
|---------|---------|------|---------|-------|
| [mgsh_sama7d65_lvgl](mgsh_sama7d65_lvgl/) | SAMA7D65 | Cortex-A7 | 800x480 WVGA | [Curiosity (EV63J76A)](https://www.microchip.com/en-us/development-tool/ev63j76a) |
| [mgsh_sam9x75_lvgl](mgsh_sam9x75_lvgl/) | SAM9X75 | ARM926EJ-S | 800x480 WVGA | [Curiosity (EV31H43A)](https://www.microchip.com/en-us/development-tool/EV31H43A) |
| [mgsh_sam9x60_lvgl](mgsh_sam9x60_lvgl/) | SAM9X60 | ARM926EJ-S | 800x480 WVGA | [Curiosity (EV40E67A)](https://www.microchip.com/en-us/development-tool/ev40e67a) |
| [mgsh_pic32mzda_lvgl](mgsh_pic32mzda_lvgl/) | PIC32MZ DA | MIPS32 M-Class | 800x480 WVGA | [Curiosity (EV87D54A)](https://www.microchip.com/en-us/development-tool/EV87D54A) |
| [mgsh_pic32cz_ca70_lvgl](mgsh_pic32cz_ca70_lvgl/) | PIC32CZ CA70 | Cortex-M7 | 480x272 WQVGA | [Curiosity Ultra (EV56T44A)](https://www.microchipdirect.com/product/EV56T44A) |
| [mgsh_pic32ck_sg_gc_lvgl](mgsh_pic32ck_sg_gc_lvgl/) | PIC32CK GC01 | Cortex-M33 | 480x272 WQVGA | [Curiosity Ultra (EV44P93A)](https://www.microchip.com/en-us/development-tool/ev44p93a) |

## Common Features

All projects include:

- **LVGL v9.5.0** integration (included as a git submodule)
- **16-bit RGB565** color depth
- **Capacitive touch** input via MaxTouch driver
- **Multiple demo applications**:
  - Widgets demo
  - Benchmark demo
  - Stress test demo
  - Music player demo
  - Flex layout demo
- **CMake build system** support
- **VS Code** project files with MPLAB Extension integration

## Software Requirements

- **IDE**: VS Code with [MPLAB Extension](https://marketplace.visualstudio.com/items?itemName=microchip.mplab)
- **Toolchain**: XC32 Compiler v5.00 or later
- **Framework**: MPLAB Harmony v3

## Getting Started

### Clone the Repository

```bash
git clone --recursive https://github.com/<your-org>/mgsh_lvgl.git
```

If you already cloned without `--recursive`, initialize the submodules:

```bash
git submodule update --init --recursive
```

### Building a Project

1. Navigate to the desired project folder (e.g., `mgsh_sam9x60_lvgl/`)
2. Open in VS Code - the MPLAB extension will detect the project automatically
3. Select the build configuration and build

Or use CMake directly:

```bash
cd <project>/_build/<project>/default
cmake --build .
```

## Repository Structure

```
mgsh_lvgl/
├── lvgl/                       # LVGL library (git submodule)
├── mgsh_sama7d65_lvgl/         # SAMA7D65 Curiosity project
├── mgsh_sam9x75_lvgl/          # SAM9X75 Curiosity project
├── mgsh_sam9x60_lvgl/          # SAM9X60 Curiosity project
├── mgsh_pic32mzda_lvgl/        # PIC32MZ DA Curiosity project
├── mgsh_pic32cz_ca70_lvgl/     # PIC32CZ CA70 Curiosity Ultra project
└── mgsh_pic32ck_sg_gc_lvgl/    # PIC32CK SG/GC Curiosity Ultra project
```

Each project folder contains:

| Path | Description |
|------|-------------|
| `*/src/` | Application source code |
| `*/src/app_lvgl.c` | LVGL display driver integration |
| `*/src/lv_conf.h` | LVGL configuration |
| `binaries/` | Bootloader binaries (MPU projects) |
| `_build/` | CMake build tree |
| `out/` | Final build artifacts |
| `.vscode/` | VS Code and MPLAB project files |

## Configuration

### Selecting a Demo

Edit `lv_conf.h` in the project's source folder to enable the desired demo:

```c
#define LV_USE_DEMO_WIDGETS     1   // Enable widgets demo
#define LV_USE_DEMO_BENCHMARK   0
#define LV_USE_DEMO_STRESS      0
#define LV_USE_DEMO_MUSIC       0
#define LV_USE_DEMO_FLEX_LAYOUT 0
```

### LVGL Memory Pool

Adjust `LV_MEM_SIZE` in `lv_conf.h` based on your application needs:

```c
#define LV_MEM_SIZE (128 * 1024U)  // 128KB heap
```

## Architecture

All projects follow the MPLAB Harmony application model:

```
main()
  └─> SYS_Initialize()     // Initialize Harmony modules
  └─> SYS_Tasks()          // Main loop
        ├─> APP_LVGL_Tasks()    // LVGL state machine
        │     ├─> lv_init()
        │     ├─> lv_display_create()
        │     ├─> lv_indev_create()
        │     └─> lv_timer_handler()
        └─> Touch Driver Tasks  // Touch input processing
```

## License

This repository contains code from:
- **Microchip Technology Inc.** - Harmony framework and drivers
- **LVGL LLC** - LVGL graphics library (MIT License)

## Resources
- [Developer Help](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-lvgl/mplab-harmony/)
- [LVGL Documentation](https://docs.lvgl.io/)
- [MPLAB Harmony v3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony)
- [Microchip Graphics Suite](https://www.microchip.com/en-us/tools-resources/develop/graphics)
- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)

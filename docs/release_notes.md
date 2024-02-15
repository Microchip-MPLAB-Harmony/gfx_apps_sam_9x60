---
title: Release notes
nav_order: 99
---

![MPLAB® Harmony Graphics Suite](./images/mhgs.png) Microchip MPLAB® Harmony 3 Graphics Application Examples for SAM 9x60 Family
# Microchip MPLAB® Harmony 3 Release Notes

## MPLAB® Harmony 3 Graphics Application Examples for SAM 9x60 Family v3.12.0

### Development kit and demo application support

Following table provides number of peripheral library examples available for different development kits.

| Development Kits  | MPLABx applications |
|:-----------------|:-------------------:|
| [SAM 9x60 Evaluation Kit](https://www.microchip.com/en-us/development-tool/DT100126) | 8 |
| [SAM9X60 Curiosity Development Board](https://www.microchip.com/en-us/development-tool/EV40E67A) | 8 |
|

## Release v3.12.0

### New Features

New features added in this release are as follows:

- Added LVGL Showcase to WVGA configuration for SAM9x60 Curiosity Board


### Known Issues

The current known issues are as follows:

- For all IAR configurations, make sure LE_DYNAMIC_VTABLES is define to 1 to ensure build success
- For proper GFX2D support, some configurations will require the data buffer memory definted in no_cache region, ie. SECTION(".region_nocache") definition for _dataBuffers in legato_renderer.c

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - [MPLAB® Code Configurator (MCC) Plugin v5.4.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin).
    - [MPLAB® Code Configurator (MCC) Core v5.6.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin).

### Dependent Components

* [GFX v3.14.0](https://github.com/Microchip-MPLAB-Harmony/gfx/releases/tag/v3.14.0)


## Release v3.11.0

### New Features

New features added in this release are as follows:

- Added Blank Quickstart (Single-Buffered) WVGA configuration for SAM9x60 Curiosity Board
- Added Blank Quickstart (Double-Buffered) WVGA configuration for SAM9x60 Curiosity Board
- Added Blank Canvas Quickstart WVGA configuration for SAM9x60 Curiosity Board
- Added Legato Canvas Quickstart WVGA configuration for SAM9x60 Curiosity Board
- Added Legato Quickstart WVGA configuration for SAM9x60 Curiosity Board


## Release v3.10.0

### New Features

New features added in this release are as follows:

- Added Burst mode support for DMA when double-buffer is enabled
- Added a camera (OV5640) example using the ISI interface
- Added support for MPLAB Code Configurator

## Release v3.9.3

### New Features

New features added in this release are as follows:

- Added Blank Quickstart (Single-Buffered) WVGA configuration
- Added Blank Quickstart (Double-Buffered) WVGA configuration
- Added Blank Canvas Quickstart WVGA configuration
- Added Legato Canvas Quickstart WVGA configuration

## Release v3.9.2

### New Features

New features added in this release are as follows:

- Added Legato Quickstart WVGA configuration with 270 Orientation


## Release v3.9.1

### Issues Fixed

- Fixed issues with packages and documentation


## Release v3.9.0

### New Features

New features added in this release are as follows:

- Added Legato Quickstart WVGA configuration for IAR Embedded WorkBench
- Added Legato Dashboard WVGA configuration with Graphics Canvas support

### Known Issues

The current known issues are as follows:

- For all IAR configurations, make sure LE_DYNAMIC_VTABLES is define to 1 to ensure build success
- For proper GFX2D support, some configurations will require the data buffer memory definted in no_cache region, ie. SECTION(".region_nocache") definition for _dataBuffers in legato_renderer.c

### Development Tools

- [MPLAB® X IDE v6.05 and above](https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® X IDE plug-ins:
    - [MPLAB® Harmony Configurator (MHC) v3.8.5](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator).
    - [MPLAB® Code Configurator (MCC) v5.2.2](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin).
- [MPLAB® XC32 C/C++ Compiler v4.20 and above](https://www.microchip.com/mplab/compilers)

### Dependent Components

* [GFX v3.12.0](https://github.com/Microchip-MPLAB-Harmony/gfx/releases/tag/v3.12.0)

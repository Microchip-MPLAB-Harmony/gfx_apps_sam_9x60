---
title: Release notes
nav_order: 99
---

![MPLAB® Harmony Graphics Suite](./docs/images/mhgs.png) Microchip MPLAB® Harmony 3 Graphics Application Examples for SAM 9x60 Family
# Microchip MPLAB® Harmony 3 Release Notes

## MPLAB® Harmony 3 Graphics Application Examples for SAM 9x60 Family v3.10.0

### Development kit and demo application support

Following table provides number of peripheral library examples available for different development kits.

| Development Kits  | MPLABx applications |
|:-----------------|:-------------------:|
| SAM 9x60 Evaluation Kit | 10 |
|

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

- [MPLAB® X IDE v6.00 and above](https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® X IDE plug-ins:
    - [MPLAB® Harmony Configurator (MHC) v3.8.3](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator).
    - [MPLAB® Code Configurator (MCC) v5.1.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin).
- [MPLAB® XC32 C/C++ Compiler v3.01 and above](https://www.microchip.com/mplab/compilers)

### Dependent Components

* [GFX v3.10.0](https://github.com/Microchip-MPLAB-Harmony/gfx/releases/tag/v3.10.0)
* [IAR Embedded WorkBench ARM v8.40.1 and above](https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)

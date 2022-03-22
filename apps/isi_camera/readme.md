---
parent: Example Applications
title: ISI Camera
nav_order: 1
---

# ISI Camera

![](./../../docs/html/ov5640_arducam.png)

This demonstration shows how to use the SAM9X60-EK Image Sensor Interface peripheral with an off-the-shelf image sensor. For this example, an arducam OV5640
Camera Module is used. The application will store the captured images to a USB storage device in TGA format. When user pushes SW1 button, a full-size ISI codec 
capture is initiated and the image is stored in the USB storage device.

|MPLABX Configuration|Board Configuration|
|:-------------------|:------------------|
|[isi\_cam\_sam9x60\_ek.X](./firmware/isi_cam_sam9x60_ek.X/readme.md)| [SAM 9x60 Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/DT100126) using the ISI interface to drive an off-the-shelf image sensor|

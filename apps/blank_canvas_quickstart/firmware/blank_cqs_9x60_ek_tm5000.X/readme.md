
![](../../../../docs/images/mhgs.png) blank\_cqs\_9x60\_ek\_tm5000.X

Defining the Architecture
-------------------------

![](../../../../docs/html/blank_canvas_sam9x60_single_buffer_arch.png)

The application calls directly into LCD Controller (LCDC) driver set up and initialization function calls that configure the LCDC. After initialization, the application transitions into the paint phase, where it uses Canvas Graphics library to draw an image to the frame buffer. Pressing down on the LCD turns on the BLUE LED and when touch is removed the BLUE LED turns off. Each time the user touches the LCD, the image displayed is switched.  

User touch input on the display panel is received thru the PCAP capacitive touch controller, which sends a notification to the Touch Input Driver. The Touch Input Driver reads the touch information over I2C and sends the touch event to the Graphics Library thru the Input System Service.

### Demonstration Features

-   Graphics Acceleration using integrated display controller (LCDC)
-   Graphics Canvas
-   Reference application for the SAM 9x60
-   24-bit color, multi-layer, WVGA (800x480) screen design
-   Time system service, timer-counter peripheral library and driver 
-   I2C and touch controller driver 

Creating the Project Graph
--------------------------

![](../../../../docs/html/blank_sam9x60_canvas_lcdc_no_gpu_wvga_pg.png)

The Project Graph diagram shows the Harmony components that are included in this application. Lines between components are drawn to satisfy components that depend on a capability that another component provides.


Building the Application
------------------------

The parent directory for this application is gfx/apps/blank\_canvas\_quickstart. To build this application, use MPLAB X IDE to open the gfx/apps/blank\_canvas\_quickstart/firmware/blank\_cqs\_9x60\_ek\_tm5000.X project folder and press F11.
A successful build will generate a harmony.bin file in dist\lcdc_rgba8888_mxt_9x60_wvga\production in the project folder.

The following table lists configuration properties:

|Project Name|BSP Used|Graphics Template Used|Description|
|:-----------|:-------|:---------------------|:----------|
|blank\_cqs\_9x60\_ek\_tm5000.X|[SAM 9x60 Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/DT100126)|None|[SAM 9x60 Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/DT100126) with [RGBA8888 GFX Interface and 5" WVGA PCAP Touch display](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/AC320005-5)|


Configuring the Hardware
------------------------

Configure the hardware as follows:

-   Connect the ribbon cable from the display to the J15 connector on the back of the SAM 9x60 Evaluation Kit board.

![](../../../../docs/html/sam_9x60_sk_display_conf1.png)

-	Take an SD Card formatted with FAT32 file system, and copy the boot.bin binary file from this [location](../../../boot_image/boot.bin). Also copy the harmony.bin file you generated from the "Building the Application" section.

-   Insert the SD card to J4 of the SAM 9X60 Evaluation kit and power up the board by connecting the power adapter to power connector J1 or a powered USB cable to the USB port J7 on the SAM 9x60 Evaluation Kit board.


Running the Demonstration
-------------------------

When power-on is successful, the demonstration will display an image similar to the picture shown below (different configurations may have slight variation in the screen aspect ratio):

![](../../../../docs/html/blank_canvas_qs.png)


Touching the LCD will turn on the blue LED and when touch is removed, the image below is displayed:

![](../../../../docs/html/blank_canvas_qs_1.png)

With every touch on the LCD, the displayed image swaps between the two pictures shown above.
* * * * *

 

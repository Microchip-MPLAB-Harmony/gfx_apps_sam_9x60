/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "definitions.h"
#include "system/input/sys_input.h"
#include "gfx/driver/gfx_driver.h"

#define APP_GFX_LAYER_WIDTH_PIXELS  800
#define APP_GFX_LAYER_HEIGHT_PIXELS 480

#define WAIT_TIME 0xFFFFF

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern const uint8_t StartWithBlankText_data[23496];
extern const uint8_t NewHarmonyLogo_data[30472];
extern const uint8_t mchp_splash[1536000];
uint8_t background[APP_GFX_LAYER_WIDTH_PIXELS * APP_GFX_LAYER_HEIGHT_PIXELS * 4];

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
gfxPixelBuffer imageBuffer;
gfxPixelBuffer textBuffer;
gfxPixelBuffer backgroundBuffer;
SYS_INP_InputListener appInputListener;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

static void touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
	LED_BLUE_On();
}

static void touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
	LED_BLUE_Off();
    LED_GREEN_Off();
    
    if (appData.lastPaintState == APP_STATE_PAINT)
        appData.state = APP_STATE_PAINT1;
    else
        appData.state = APP_STATE_PAINT;
}

static void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
	LED_GREEN_On();	
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void APP_BlitBuffer(int32_t x,
                    int32_t y,
                    gfxPixelBuffer* buf)
{
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;

    rowSize = buf->size.width * sizeof(uint32_t);

    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&backgroundBuffer, x, y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
    Maintains state machine of APP.
 
 */
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;    
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

 */


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    unsigned int canvasId = 0;
    unsigned int layerId = 0;
    
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            
			// Register the input event handlers
			appInputListener.handleTouchDown = &touchDownHandler;
			appInputListener.handleTouchUp = &touchUpHandler;
			appInputListener.handleTouchMove = &touchMoveHandler;
			SYS_INP_AddListener(&appInputListener);

			gfxPixelBufferCreate(341,
                    320,
                    GFX_COLOR_MODE_RGBA_8888,
                    NewHarmonyLogo_data,
                    &imageBuffer);
            
            gfxPixelBufferCreate(267,
                    22,
                    GFX_COLOR_MODE_RGBA_8888,
                    StartWithBlankText_data,
                    &textBuffer);
                
            gfxPixelBufferCreate(APP_GFX_LAYER_WIDTH_PIXELS,
                    APP_GFX_LAYER_HEIGHT_PIXELS,
                    GFX_COLOR_MODE_RGBA_8888,
                    background,
                    &backgroundBuffer);
            
            //Assemble first screen
            //Paint background black
            memset(&background, 0x0F00, sizeof(background)); 

            //Place the text
            APP_BlitBuffer(250, 380, &textBuffer);
            
            //Place the logo
            APP_BlitBuffer(210, 30, &imageBuffer);            
            
            /* Initialize canvas objects */
            gfxcSetLayer(canvasId, layerId);
            gfxcSetWindowSize(canvasId, 800, 480);
            gfxcSetWindowPosition(canvasId, 0, 0);

            gfxcSetPixelBuffer(canvasId,
                               800,
                               480,
                               GFX_COLOR_MODE_RGBA_8888,
                               (void *)background);

            gfxcShowCanvas(canvasId);
            gfxcCanvasUpdate(canvasId);

            if (appInitialized)
            {
                appData.state = APP_STATE_PAINT;
            }
            break;
        }

        case APP_STATE_PAINT:
        {
            //Point canvas to first frame
            gfxcSetPixelBuffer(canvasId,
                               800,
                               480,
                               GFX_COLOR_MODE_RGBA_8888,
                               (void *)background);
            gfxcCanvasUpdate(canvasId);
            
            appData.lastPaintState = appData.state;
            appData.state = APP_STATE_PROCESS;
    
            break;
        }
        case APP_STATE_PAINT1:
        {
            //Point canvas to second frame
            gfxcSetPixelBuffer(canvasId,
                   800,
                   480,
                   GFX_COLOR_MODE_RGBA_8888,
                   (void *)mchp_splash);
            gfxcCanvasUpdate(canvasId);

            appData.lastPaintState = appData.state;
            appData.state = APP_STATE_PROCESS;
    
            break;
        }
        case APP_STATE_PROCESS:
        {
            //Do application processing
            break;
        }
        case APP_STATE_ERROR:
        /* The default state should never be executed. */
        default:
        {
            LED_RED_On();
            //Light the LED for error
            break;
        }
    }
}


 
/*******************************************************************************
 End of File
 */

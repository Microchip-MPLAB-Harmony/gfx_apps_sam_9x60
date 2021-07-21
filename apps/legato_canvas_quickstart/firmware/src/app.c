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
#include "gfx/canvas/gfx_canvas_api.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

leWidgetEventFilter touchEventReceiver;
bool layerMoveAnimDone = true;


#define BACKGROUND_CANVAS_XPOS    0
#define BACKGROUND_CANVAS_YPOS    0
#define BACKGROUND_CANVAS_WIDTH   800
#define BACKGROUND_CANVAS_HEIGHT  480

#define MHGS_LOGO_CANVAS_XPOS    0
#define MHGS_LOGO_CANVAS_YPOS    100
#define MHGS_LOGO_CANVAS_WIDTH   800
#define MHGS_LOGO_CANVAS_HEIGHT  320

#define QUICKSTART_BUTTON_CANVAS_XPOS    370
#define QUICKSTART_BUTTON_CANVAS_YPOS    200
#define QUICKSTART_BUTTON_CANVAS_WIDTH   430
#define QUICKSTART_BUTTON_CANVAS_HEIGHT  180


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

leBool appTouchEventReceiver(leWidget* target, leWidgetEvent* evt, void* data)
{
    leWidgetEvent_TouchMove* touchMoveEvent = (leWidgetEvent_TouchMove*)evt;
    
    if (evt->id != LE_EVENT_TOUCH_MOVE)
        return LE_FALSE;
    
    if (layerMoveAnimDone == false)
        return LE_TRUE;
 
    gfxcSetWindowPosition(MHGS_LOGO_CANVAS_ID, touchMoveEvent->x, touchMoveEvent->y);
    gfxcCanvasUpdate(MHGS_LOGO_CANVAS_ID);
    
    return LE_TRUE;   
}

void Screen0_OnShow(void)
{
    Screen0_ButtonTouchReceiver->fn->installEventFilter(Screen0_ButtonTouchReceiver, touchEventReceiver);
}

void Screen0_OnUpdate(void)
{
    
}

void event_Screen0_ButtonWidget1_OnReleased(leButtonWidget* btn)
{
    if (layerMoveAnimDone == false)
        return;
 
    int x, y = 0;
    
    if (gfxcGetWindowPosition(MHGS_LOGO_CANVAS_ID, &x, &y) == GFX_SUCCESS)
    {
        gfxcStartEffectMove(MHGS_LOGO_CANVAS_ID,
                GFXC_FX_MOVE_DEC,
                x,
                y,
                MHGS_LOGO_CANVAS_XPOS,
                MHGS_LOGO_CANVAS_YPOS,
                10);

        layerMoveAnimDone = false;
    }
}

void app_CanvasEffect_CallBack(unsigned int canvasID,
                                GFXC_FX_TYPE effect,
                                GFXC_FX_STATUS status,
                                void * parm)
{
    switch (canvasID)
    {
        case MHGS_LOGO_CANVAS_ID:
            layerMoveAnimDone = (status == GFXC_FX_DONE) ? true : false;
            break;
        case BACKGROUND_CANVAS_ID:
        case QUICKSTART_BUTTON_CANVAS_ID:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void appSetLayerFrame(uint32_t layerID, 
                      uint32_t addr,
                      uint32_t x,
                      uint32_t y,
                      uint32_t width,
                      uint32_t height)
{
    gfxcSetPixelBuffer(layerID,
                       width,
                       height,
                       GFX_COLOR_MODE_RGBA_8888,
                       (void *) addr);
    
    gfxcSetWindowPosition(layerID, x, y);
    gfxcSetWindowSize(layerID, width, height);
    gfxcCanvasUpdate(layerID);
}

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

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    touchEventReceiver.filterEvent = appTouchEventReceiver;
    touchEventReceiver.data = &appData;
    
    gfxcSetEffectsCallback(MHGS_LOGO_CANVAS_ID, app_CanvasEffect_CallBack, NULL);
    
    gfxcSetLayer(BACKGROUND_CANVAS_ID, BACKGROUND_LAYER_ID);
    gfxcSetLayer(MHGS_LOGO_CANVAS_ID, MHGS_LOGO_LAYER_ID);
    gfxcSetLayer(QUICKSTART_BUTTON_CANVAS_ID, QUICKSTART_BUTTON_LAYER_ID);

    gfxcSetWindowPosition(BACKGROUND_CANVAS_ID, BACKGROUND_CANVAS_XPOS, BACKGROUND_CANVAS_YPOS);
    gfxcSetWindowSize(BACKGROUND_CANVAS_ID, BACKGROUND_CANVAS_WIDTH, BACKGROUND_CANVAS_HEIGHT);

    gfxcSetWindowPosition(MHGS_LOGO_CANVAS_ID, MHGS_LOGO_CANVAS_XPOS, MHGS_LOGO_CANVAS_YPOS);
    gfxcSetWindowSize(MHGS_LOGO_CANVAS_ID, MHGS_LOGO_CANVAS_WIDTH, MHGS_LOGO_CANVAS_HEIGHT);

    gfxcSetWindowPosition(QUICKSTART_BUTTON_CANVAS_ID, QUICKSTART_BUTTON_CANVAS_XPOS, QUICKSTART_BUTTON_CANVAS_YPOS);
    gfxcSetWindowSize(QUICKSTART_BUTTON_CANVAS_ID, QUICKSTART_BUTTON_CANVAS_WIDTH, QUICKSTART_BUTTON_CANVAS_HEIGHT);
    
    gfxcShowCanvas(BACKGROUND_CANVAS_ID);
    gfxcShowCanvas(MHGS_LOGO_CANVAS_ID);
    gfxcShowCanvas(QUICKSTART_BUTTON_CANVAS_ID);
    
    gfxcCanvasUpdate(BACKGROUND_CANVAS_ID);
    gfxcCanvasUpdate(MHGS_LOGO_CANVAS_ID);
    gfxcCanvasUpdate(QUICKSTART_BUTTON_CANVAS_ID);    
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {

            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */

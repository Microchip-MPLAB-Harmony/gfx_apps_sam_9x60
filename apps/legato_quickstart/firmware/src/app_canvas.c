/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_canvas.c

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

#include "app_canvas.h"
#include "gfx/canvas/gfx_canvas_api.h"

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
    This structure should be initialized by the APP_CANVAS_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_CANVAS_DATA app_canvasData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_CANVAS_Initialize ( void )

  Remarks:
    See prototype in app_canvas.h.
 */

void APP_CANVAS_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_canvasData.state = APP_CANVAS_STATE_INIT;

    gfxcSetLayer(0, 0);
    gfxcSetLayer(1, 1);
    gfxcSetLayer(2, 2);

    gfxcShowCanvas(0);
    gfxcShowCanvas(1);
    gfxcShowCanvas(2);

    gfxcCanvasUpdate(0);
    gfxcCanvasUpdate(1);
    gfxcCanvasUpdate(2);
}


/******************************************************************************
  Function:
    void APP_CANVAS_Tasks ( void )

  Remarks:
    See prototype in app_canvas.h.
 */

void APP_CANVAS_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_canvasData.state )
    {
        /* Application's initial state. */
        case APP_CANVAS_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app_canvasData.state = APP_CANVAS_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_CANVAS_STATE_SERVICE_TASKS:
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

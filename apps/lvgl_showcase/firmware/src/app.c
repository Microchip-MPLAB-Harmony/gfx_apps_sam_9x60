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
#include "third_party/lv_conf.h"
#include "third_party/lvgl/lvgl.h"
#include "gfx/driver/gfx_driver.h"
#include "third_party/lvgl/demos/lv_demos.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* LVGL Parameters */
#define LV_TICK_INC_VAL_MS  1

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

/* LVGL Variables */
/* Display driver instance */
lv_disp_drv_t lv_disp_drv;

/* Display instance */
lv_disp_t * lv_disp;

/* Display buffer parameters */
lv_disp_draw_buf_t lv_disp_draw_buf;

/* Input driver instance */
lv_indev_drv_t lv_indev_drv;

/* Input instance */
lv_indev_t * lv_indev;

/* Input System Service */
/* Input listener instance */
SYS_INP_InputListener input_listener;

/* Touch point x */
uint32_t touch_x;

/* Touch point y */
uint32_t touch_y;

/* Touch state */
bool touch_pressed;

/* Scratch Buffer */
__attribute__ ((section(".region_nocache"), aligned (32))) uint16_t scratch_buff[800 * 480];

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* Static Functions */
static void touchDownHandler(const SYS_INP_TouchStateEvent * const evt) {
        touch_pressed = true;
        touch_x = evt->x;
        touch_y = evt->y;
}

static void touchUpHandler(const SYS_INP_TouchStateEvent * const evt) {
        touch_pressed = false;
        touch_x = evt->x;
        touch_y = evt->y;
}

static void touchMoveHandler(const SYS_INP_TouchMoveEvent * const evt) {
        touch_x = evt->x;
        touch_y = evt->y;
}

static void lv_tick_inc_cb(uintptr_t _) {
    lv_tick_inc(LV_TICK_INC_VAL_MS);
}

static void lv_disp_drv_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
    gfxPixelBuffer dstBuf;
   
    memset(&dstBuf, 0, sizeof(gfxPixelBuffer));
    
    gfxPixelBufferCreate((area->x2 + 1) - area->x1,
                         (area->y2 + 1) - area->y1,
                         GFX_COLOR_MODE_RGB_565, color_p, &dstBuf);
    
    DRV_LCDC_BlitBuffer(area->x1, area->y1, &dstBuf);
    
    lv_disp_flush_ready(disp_drv);
}

static void lv_indev_drv_read_cb(lv_indev_drv_t * _, lv_indev_data_t * indev_data) {
    if (touch_pressed) {
        indev_data->point.x = touch_x;
        indev_data->point.y = touch_y;
        indev_data->state = LV_INDEV_STATE_PR;
    } else {
        indev_data->point.x = touch_x;
        indev_data->point.y = touch_y;
        indev_data->state = LV_INDEV_STATE_REL;
    }
}

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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
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

            /* Initialize Input System Service */
            input_listener.handleTouchDown = &touchDownHandler;
            input_listener.handleTouchUp = &touchUpHandler;
            input_listener.handleTouchMove = &touchMoveHandler;
            SYS_INP_AddListener(&input_listener);
            touch_pressed = false;
    
            /* Initialize LVGL */
            lv_init();
            
            /* Set Active Layer */
            gfxIOCTLArg_Value ioctlArg;
            ioctlArg.value.v_uint = 0;
            DRV_LCDC_IOCTL(GFX_IOCTL_SET_ACTIVE_LAYER, &ioctlArg);  
            ioctlArg.value.v_colormode = GFX_COLOR_MODE_RGB_565;
            DRV_LCDC_IOCTL(GFX_IOCTL_SET_LAYER_COLOR_MODE, &ioctlArg);
            
            /* Register Display in LVGL */
            lv_disp_draw_buf_init(&lv_disp_draw_buf,
                                  scratch_buff,
                                  NULL,
                                  800 * 480);
            
            lv_disp_drv_init(&lv_disp_drv);
            lv_disp_drv.hor_res = 800;
            lv_disp_drv.ver_res = 480;
            lv_disp_drv.draw_buf = &lv_disp_draw_buf;
            lv_disp_drv.flush_cb = lv_disp_drv_flush_cb;
            lv_disp = lv_disp_drv_register(&lv_disp_drv);

            /* Register Input in LVGL */
            lv_indev_drv_init(&lv_indev_drv);
            lv_indev_drv.type = LV_INDEV_TYPE_POINTER;
            lv_indev_drv.read_cb = lv_indev_drv_read_cb;
            lv_indev = lv_indev_drv_register(&lv_indev_drv);

            /* Periodic tick for LVGL */
            SYS_TIME_CallbackRegisterMS(lv_tick_inc_cb, 0, LV_TICK_INC_VAL_MS,
                    SYS_TIME_PERIODIC);
            
            /* Demo */
            #if LV_USE_DEMO_WIDGETS
            lv_demo_widgets();
            #elif LV_USE_DEMO_BENCHMARK
            lv_demo_benchmark();
            #elif LV_USE_DEMO_STRESS
            lv_demo_stress();
            #elif LV_USE_DEMO_MUSIC
            lv_demo_music();
            #endif
            
            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            lv_task_handler();
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

#include "demo.h"

#include "driver/isi/drv_isi.h"
#include "app.h"

#include "definitions.h"
#include "bsp/bsp.h"

#include <string.h>

#define USB_MOUNT_NAME       "/mnt/myDrive1"
#define USB_DEV_NAME         "/dev/mmcblka1"

#define PREVIEW_IMAGE_WIDTH  (320)
#define PREVIEW_IMAGE_HEIGHT (240)
#define PREVIEW_PIXEL_SIZE   (2)
#define PREVIEW_IMAGE_SIZE   (CAMERA_IMAGE_WIDTH * CAMERA_IMAGE_HEIGHT * CAMERA_PIXEL_SIZE)

#define CAPTURE_IMAGE_WIDTH  (640)
#define CAPTURE_IMAGE_HEIGHT (480)
#define CAPTURE_PIXEL_SIZE   (2)
#define CAPTURE_IMAGE_SIZE   (CAPTURE_IMAGE_WIDTH * CAPTURE_IMAGE_HEIGHT * CAPTURE_PIXEL_SIZE)

typedef enum
{
    STATE_INIT = 0,
    STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
    STATE_MOUNT_FS,
    STATE_START_PREVIEW_MODE,
    STATE_WRITE_PREVIEW_FRAME,        
    STATE_REQUEST_CAPTURE,
    STATE_WAIT_CAPTURE,
    STATE_WRITE_CAPTURE,
    STATE_UNMOUNT,        
    STATE_IDLE,       
    STATE_DONE        
} DemoState;

static volatile DemoState _state = STATE_INIT;

static uint32_t usbDeviceConnected;
static uint32_t previewFrameIdx = 0;
static uint32_t codecFrameIdx = 0;
static volatile uint32_t _isMounted;
static volatile uint32_t _switchTime;
static volatile uint32_t _previewFrameReady;
static volatile uint32_t _captureRequested;
static volatile uint32_t _captureReady;
static volatile uint32_t _shouldUnmount;
static volatile uint8_t* _lastPreviewBuffer;
static volatile uint8_t* _lastCodecBuffer;

static void writePreviewFrame(uint16_t* buf,
                              uint32_t w,
                              uint32_t h)
{
    int32_t xa, xb, ya, yb;
    char fileName[32];
    uint8_t header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0};
    uint32_t idx, val;
    int32_t r, c;
    uint8_t clr[3];
    
    sprintf(fileName,
            "/mnt/myDrive1/preview_frame_%d.tga",
            previewFrameIdx++);
    
    xa = w % 256;
    xb = (w-xa) / 256;
    ya = h % 256;
    yb = (h-ya) / 256;//assemble the header
    header[12] = (uint8_t)xa;
    header[13] = (uint8_t)xb;
    header[14] = (uint8_t)ya;
    header[15] = (uint8_t)yb;

    // write header and data to file
    SYS_FS_HANDLE handle = SYS_FS_FileOpen(fileName, (SYS_FS_FILE_OPEN_WRITE_PLUS));

    SYS_FS_FileWrite(handle, (const void *) header, sizeof(header));
    
    // convert rgb565 to rgb888
    for(r = h - 1; r >= 0; --r)
    {
        for(c = 0; c < w; ++c)
        {
            val = buf[(r * w) + c];

            clr[2] = (val & 0xF800) >> 8;
            clr[1] = (val & 0x7E0) >> 3;
            clr[0] = (val & 0x1F) << 3;

            SYS_FS_FileWrite(handle, (const void *)clr, 3);
        }
    }
    
    SYS_FS_FileClose(handle);
}

static void _writeCodecFrame(void* buf)
{
    char fileName[32];
    
    sprintf(fileName,
            "/mnt/myDrive1/codec_frame_%d.yuv",
            codecFrameIdx++);
    
    // write header and data to file
    SYS_FS_HANDLE handle = SYS_FS_FileOpen(fileName, (SYS_FS_FILE_OPEN_WRITE_PLUS));

    SYS_FS_FileWrite(handle, buf, CAPTURE_IMAGE_SIZE);
    
    SYS_FS_FileClose(handle);
}

static void _isiPreviewFrameReady(uint8_t* buf)
{
    DRV_ISI_PreviewPathDisable();
    
    _lastPreviewBuffer = buf;
    
    _previewFrameReady = 1;
}

static void _isiCodecFrameReady(uint8_t* buf)
{   
    _lastCodecBuffer = buf;
    
    _captureReady = 1;
}

static USB_HOST_EVENT_RESPONSE USBHostEventHandler(USB_HOST_EVENT event,
                                                   void* eventData,
                                                   uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void SYSFSEventHandler(SYS_FS_EVENT event,
                       void* eventData,
                       uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
        {
            if(strcmp((const char *)eventData, USB_MOUNT_NAME) == 0)
            {
                usbDeviceConnected = 1;
            }
            
            break;
        }    
        case SYS_FS_EVENT_UNMOUNT:
        {
            if(strcmp((const char *)eventData, USB_MOUNT_NAME) == 0)
            {
                usbDeviceConnected = 0;
                
                LED_GREEN_Off();
                LED_RED_On();
                
                _state = STATE_DONE;
            }
            
            break;
        }
        default:
            break;
    }
}

static void switchInterruptCB(PIO_PIN pin, uintptr_t context)
{
    uint32_t timeVal = SYS_TIME_CounterGet();
    uint32_t ms = 0;
    
    if(SWITCH_AL_PD18_Get() == SWITCH_AL_PD18_STATE_PRESSED)
    {
        LED_BLUE_On();
    }
    
    if(_isMounted == 1)
    {
        if(SWITCH_AL_PD18_Get() == SWITCH_AL_PD18_STATE_PRESSED)
        {
            _switchTime = timeVal;
        }
        else
        {
            ms = SYS_TIME_CountToMS(timeVal) - SYS_TIME_CountToMS(_switchTime);

            if(ms >= 3000)
            {
                _shouldUnmount = 1;
            }
            else
            {
                _captureRequested = 1;
            }
        }
    }
    
    if(SWITCH_AL_PD18_Get() == SWITCH_AL_PD18_STATE_RELEASED)
    {
        LED_BLUE_Off();
    }
}

void demoInit(void)
{
    PIO_PinInterruptEnable(PIO_PIN_PD18);

    PIO_PinInterruptCallbackRegister(PIO_PIN_PD18,
                                     switchInterruptCB,
                                     0);
    
    usbDeviceConnected = 0;
    previewFrameIdx = 0;
    _isMounted = 0;
    _switchTime = 0;
    _previewFrameReady = 0;
    _shouldUnmount = 0;
    _lastPreviewBuffer = NULL;
    
    _state = STATE_INIT;
}

void demoTasks(void)
{
    if(_shouldUnmount == 1)
    {
        _state = STATE_UNMOUNT;
    }
    else if(_captureRequested == 1 && _state == STATE_IDLE)
    {
        _state = STATE_REQUEST_CAPTURE;
    }
    else if(_previewFrameReady == 1 && _state == STATE_IDLE)
    {
        _state = STATE_WRITE_PREVIEW_FRAME;
    }
    
    switch(_state)
    {
        case STATE_INIT:
        {
            LED_RED_On();
            LED_GREEN_Off();
            
            usbDeviceConnected = 0;
            previewFrameIdx = 0;
            _isMounted = 0;
            _switchTime = 0;
            _previewFrameReady = 0;
            _shouldUnmount = 0;
            _lastPreviewBuffer = NULL;
                       
            // set the event handler and enable the bus
            SYS_FS_EventHandlerSet(&SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(&USBHostEventHandler, 0);
            USB_HOST_BusEnable(USB_HOST_BUS_ALL);
            
            _state = STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            
            break;
        }
		
        case STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
            if(USB_HOST_BusIsEnabled(USB_HOST_BUS_ALL))
            {
                _state = STATE_MOUNT_FS;
            }
            
            break; 
        }
        case STATE_MOUNT_FS:
        {
            if(usbDeviceConnected == 1)
            {
                _isMounted = 1;
                
                LED_RED_Off();
                LED_GREEN_On();
                
                _state = STATE_START_PREVIEW_MODE;
            }
            
            break;
        }
        case STATE_START_PREVIEW_MODE:
        {
            DRV_ISI_SetPreviewFrameReadyCallback(_isiPreviewFrameReady);
            
            DRV_ISI_PreviewPathEnable();
            
            _state = STATE_IDLE;
            
            break;
        }
        case STATE_WRITE_PREVIEW_FRAME:
        {
            writePreviewFrame((uint16_t*)_lastPreviewBuffer,
                      PREVIEW_IMAGE_WIDTH,
                      PREVIEW_IMAGE_HEIGHT);
    
            DRV_ISI_PreviewPathEnable();
            
            _state = STATE_IDLE;
            
            break;
        }
        case STATE_REQUEST_CAPTURE:
        {
            DRV_ISI_PreviewPathDisable();
            
            DRV_ISI_SetCodecFrameReadyCallback(_isiCodecFrameReady);
            
            DRV_ISI_CodecPathEnable();
            
            _state = STATE_WAIT_CAPTURE;
            
            break;
        }
        case STATE_WAIT_CAPTURE:
        {
            if(_captureReady == 1)
            {
                _state = STATE_WRITE_CAPTURE;
            }
            
            break;
        }
        case STATE_WRITE_CAPTURE:
        {
            _writeCodecFrame((void*)_lastCodecBuffer);
            
            _captureReady = 0;
            _captureRequested = 0;
            
            DRV_ISI_PreviewPathEnable();
            
            _state = STATE_IDLE;
            
            break;
        }
        case STATE_UNMOUNT:
        {
            DRV_ISI_PreviewPathDisable();
            
            SYS_FS_Unmount(USB_MOUNT_NAME);
            
            _isMounted = 0;
            _shouldUnmount = 0;
            usbDeviceConnected = 0;
                
            LED_GREEN_Off();
            LED_RED_On();
            
            _state = STATE_INIT;
            
            break;
        }
        case STATE_IDLE:
        case STATE_DONE:    
        {
            break;
        }
    }
}


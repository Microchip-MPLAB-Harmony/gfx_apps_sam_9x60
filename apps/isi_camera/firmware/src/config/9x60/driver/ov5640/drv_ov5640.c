#include "driver/ov5640/drv_ov5640.h"

#include "driver/ov5640/drv_ov5640_config.h"
#include "driver/ov5640/drv_ov5640_regs.h"
#include "driver/i2c/drv_i2c.h"
#include "peripheral/pio/plib_pio.h"
#include "system/time/sys_time.h"

#define OV5640_I2C_ADDR                 (0x3C)
#define OV5640_CHIP_ID                  (0x5640)

/* OV5640 Regiser Addresses */
#define OV5640_SYSTEM_CTRL0             (0x3008)
#define OV5640_CHIP_ID_HIGH_BYTE        (0x300A)
#define OV5640_CHIP_ID_LOW_BYTE         (0x300B)
#define OV5640_POLARITY_CTRL00          (0x4740)
#define OV5640_AUTOFOCUS_CMD_MAIN       (0x3022)
#define OV5640_AUTOFOCUS_CMD_ACK        (0x3023)

/* OV5640 Register Masks */
#define OV5640_SYSTEM_RESET_MASK        (0x80)
#define OV5640_SYSTEM_POWER_DOWN_MASK   (0x40)

#define OV5640_AF_SINGLE_TRIGGER        (0x03)
#define OV5640_AF_CONT_TRIGGER          (0x04)
#define OV5640_AF_RELAUNCH_ZONE_CONFIG  (0x12)

#define OV5640_AF_FW_ADDRESS            (0x8000)

typedef enum
{
    STATE_UNINITIALIZED,
    STATE_INITIALIZED,
    STATE_WAITING_FOR_TRANSFER,
    STATE_WAIT,
} DriverState;

static DriverState state;
static DRV_HANDLE drvI2CHandle;

static inline void _sleep(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

static DRV_I2C_TRANSFER_HANDLE _transferHandle;
static volatile bool _transferLock;
static volatile bool _transferSuccess;

static void _i2cEventHandler(DRV_I2C_TRANSFER_EVENT evt,
                             DRV_I2C_TRANSFER_HANDLE hdl,
                             uintptr_t data)
{
    _transferSuccess = (evt == DRV_I2C_TRANSFER_EVENT_COMPLETE);
    
    _transferLock = false;
}

static bool _read(uint16_t addr,
                  uint8_t* readBuf,
                  uint32_t sz)
{
    uint8_t addrBuf[2];

    int32_t status = 0;

    addrBuf[0] = (addr & 0x7F00) >> 8;
    addrBuf[1] = addr & 0xFF;
    
    _transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    _transferLock = true;
    
    DRV_I2C_TransferEventHandlerSet(drvI2CHandle,
                                    _i2cEventHandler,
                                    0);
    
    DRV_I2C_WriteReadTransferAdd(drvI2CHandle, 
                              OV5640_I2C_ADDR,
                              addrBuf,
                              2,
                              readBuf,
                              sz,
                              &_transferHandle);
    
    if(_transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID)
        return false;
    
    while(_transferLock == true);
    
    return _transferSuccess;
}

static bool _write(uint16_t addr,
                   uint8_t val)
{
    uint8_t buf[3];
    buf[0] = (addr & 0x7F00) >> 8;
    buf[1] = (addr & 0xFF);
    buf[2] = val;
    uint32_t timeout = 0;
    
    _transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    _transferLock = true;
    
    DRV_I2C_TransferEventHandlerSet(drvI2CHandle,
                                    _i2cEventHandler,
                                    0);
    
    DRV_I2C_WriteTransferAdd(drvI2CHandle,
                             OV5640_I2C_ADDR,
                             buf,
                             3,
                             &_transferHandle);
    
    if(_transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID)
        return false;
    
    while(_transferLock == true)
    {
        timeout += 1;
        
        if(timeout == 100000)
        {
            break;
        }
    }
    
    return _transferSuccess;
}

static bool _writeTable(const RegVal* vals)
{
    uint32_t i = 0;
    
    while(vals[i].address != 0)
    {
        if(_write(vals[i].address,
                  vals[i].value) == false)
            return false;
        
        i += 1;
        
        _sleep(10);
    }
    
    return true;
}

static bool _writeAFFirmware(void)
{
    _transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    _transferLock = true;
    
    DRV_I2C_TransferEventHandlerSet(drvI2CHandle,
                                    _i2cEventHandler,
                                    0);
    
    DRV_I2C_WriteTransferAdd(drvI2CHandle,
                             OV5640_I2C_ADDR,
                             (void*)_afinitTbl,
                             sizeof(_afinitTbl),
                             &_transferHandle);
    
    if(_transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID)
        return false;
    
    while(_transferLock == true);
    
    return _transferSuccess;
}

bool _verifyChipID(void)
{
    uint8_t buf[2];
    uint16_t id;

    buf[0] = 0;
    buf[1] = 0;
    
    _read(OV5640_CHIP_ID_HIGH_BYTE,
          buf,
          2);
    
    id = buf[1];
    id |= buf[0] << 8;
    
    if(id != OV5640_CHIP_ID)
        return false;
    
    return true;
}

static bool _initialize(void)
{
    drvI2CHandle = DRV_I2C_Open(0, DRV_IO_INTENT_READWRITE);

    _sleep(20);

    _write(0x3103, 0x11); // sysclk from pad
    
    _sleep(20);

    _write(0x3008, 0x82); // software reset

    _sleep(20);

    if(_verifyChipID() == false)
    {
        return false;
    }

    _sleep(20);

    _writeTable(initVals);

    _sleep(100);

    _writeAFFirmware();

    _sleep(5);

    return true;
}

int32_t DRV_OV5640_Init(void)
{
    _sleep(20);
    
    BSP_OV5640_RESET_Clear();
    BSP_OV5640_PDWN_Clear();
    
    _sleep(1);
    
    BSP_OV5640_RESET_Set();
    
    _initialize();
    
    return 0;
}

void DRV_OV5640_Tasks(void)
{    
    switch(state)
    {
        case STATE_UNINITIALIZED:
        {  
            state = STATE_INITIALIZED;
            
            break;
        }
        default:
        {
            break;
        }
    }
}

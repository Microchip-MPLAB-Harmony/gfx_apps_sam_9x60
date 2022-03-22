#ifndef DRV_OV5640_H
#define DRV_OV5640_H

#include "driver/driver.h"
#include "driver/driver_common.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END
    
typedef enum
{
    DRV_OV5640_SENSORMODE_PREVIEW,
    DRV_OV5640_SENSORMODE_VIDEO
} DRV_OV5640_SensorMode;

int32_t DRV_OV5640_Init(void);

void DRV_OV5640_Tasks(void);

DRV_OV5640_SensorMode DRV_OV5640_GetMode(void);

bool DRV_OV5640_SetMode(DRV_OV5640_SensorMode mode);

bool DRV_OV5640_Capture(void);


    
// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END    
    
    
#endif // DRV_OV5640
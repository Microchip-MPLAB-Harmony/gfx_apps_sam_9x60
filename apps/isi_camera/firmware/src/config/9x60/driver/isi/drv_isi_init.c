//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

#include "driver/isi/drv_isi.h"

#include <stdint.h>
#include <stddef.h>

#include <toolchain_specifics.h>

#include "system/int/sys_int.h"

#define ISI_PREVIEW_WIDTH    320
#define ISI_PREVIEW_HEIGHT   240
#define ISI_PREVIEW_SIZE     2

#define ISI_CAPTURE_WIDTH    640
#define ISI_CAPTURE_HEIGHT   480
#define ISI_CAPTURE_SIZE     2

static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer0[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];
static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer1[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];
static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer2[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];

static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer0[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];
static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer1[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];
static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer2[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];

void DRV_ISI_Config(void)
{
	uint32_t dec;
	DRV_ISI_YCCtoRGBConfiguration YCCToRGBMatrix;
	
    DRV_ISI_Reset();
    
    PLIB_ISI_DMA_CHER_P_CH_EN_Set(0);
    PLIB_ISI_DMA_CHER_C_CH_EN_Set(0);
    
    DRV_ISI_SetFrameSkip(8);
	DRV_ISI_SetInputColorMode(DRV_ISI_INPCOLORMODE_YCC_422_MODE2);

	DRV_ISI_SetInputSize(ISI_CAPTURE_WIDTH, ISI_CAPTURE_HEIGHT);
	DRV_ISI_SetPreviewSize(ISI_PREVIEW_WIDTH, ISI_PREVIEW_HEIGHT);
	
	dec = DRV_ISI_CalculateDecimationFactor(ISI_CAPTURE_WIDTH,
	                                        ISI_CAPTURE_HEIGHT,
	                                        ISI_PREVIEW_WIDTH,
	                                        ISI_PREVIEW_HEIGHT);
	
	DRV_ISI_SetDecimationFactor(dec);
	
	YCCToRGBMatrix.c0 = 149;
	YCCToRGBMatrix.c1 = 255;
	YCCToRGBMatrix.c2 = 104;
	YCCToRGBMatrix.c3 = 50;
	YCCToRGBMatrix.c4 = 204;
	YCCToRGBMatrix.crOffset = 1;
	YCCToRGBMatrix.cbOffset = 1;
	YCCToRGBMatrix.yOffset = 1;
	
	DRV_ISI_SetYCCtoRGBConversionConfiguration(&YCCToRGBMatrix);
	
		
	DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer0);
    DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer1);
    DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer2);

	DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer0);
    DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer1);
    DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer2);
}

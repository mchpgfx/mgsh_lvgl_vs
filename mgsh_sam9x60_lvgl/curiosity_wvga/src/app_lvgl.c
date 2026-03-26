// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_lvgl.h"
#include "definitions.h"
#include "gfx/driver/gfx_driver.h"
#include "gfx/driver/controller/lcdc/drv_gfx_lcdc.h"

#include "lvgl/lvgl.h"
#include "lv_conf.h"
#include "lvgl/demos/lv_demos.h"

/* LVGL Parameters */
#define LV_UNCACHED_BUFFER  1
#define LV_TICK_INC_VAL_MS  1
#define LV_TASK_INC_VAL_MS  LV_DEF_REFR_PERIOD
#define DISP_WIDTH 800
#define DISP_HEIGHT 480

#define APP_USE_DIRECT_RENDER_MODE 0

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
    This structure should be initialized by the APP_LVGL_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/
APP_LVGL_DATA app_lvglData;

/* Scratch Buffer */
#if LV_UNCACHED_BUFFER
__attribute__ ((section(".region_nocache"), aligned (32))) uint16_t buff[DISP_WIDTH * DISP_HEIGHT];
#else
__attribute__ ((aligned (32))) uint32_t buff[800 * 480];
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Touch down event handler callback
 *
 * Called by the Input System Service when a touch press is detected.
 * Updates the application touch state to pressed and stores the touch coordinates.
 *
 * @param evt Pointer to the touch state event containing x,y coordinates
 */
static void touchDownHandler(const SYS_INP_TouchStateEvent * const evt)
{
        app_lvglData.touch_pressed = true;
        app_lvglData.touch_x = evt->x;
        app_lvglData.touch_y = evt->y;
}

/**
 * @brief Touch up event handler callback
 *
 * Called by the Input System Service when a touch release is detected.
 * Updates the application touch state to released and stores the final coordinates.
 *
 * @param evt Pointer to the touch state event containing x,y coordinates
 */
static void touchUpHandler(const SYS_INP_TouchStateEvent * const evt)
{
        app_lvglData.touch_pressed = false;
        app_lvglData.touch_x = evt->x;
        app_lvglData.touch_y = evt->y;
}

/**
 * @brief Touch move event handler callback
 *
 * Called by the Input System Service when touch movement is detected.
 * Updates the stored touch coordinates for drag operations.
 *
 * @param evt Pointer to the touch move event containing new x,y coordinates
 */
static void touchMoveHandler(const SYS_INP_TouchMoveEvent * const evt)
{
        app_lvglData.touch_x = evt->x;
        app_lvglData.touch_y = evt->y;
}

/**
 * @brief System timer callback for LVGL tick increment
 *
 * Registered with SYS_TIME to be called periodically. Increments the
 * application tick counter used by LVGL for timing and animations.
 *
 * @param _ Unused callback context parameter
 */
static void lv_tick_inc_cb(uintptr_t _)
{
    app_lvglData.time_ticks += LV_TICK_INC_VAL_MS;
}

/**
 * @brief Get current LVGL tick count
 *
 * Callback registered with lv_tick_set_cb() to provide LVGL with
 * the current system tick count in milliseconds.
 *
 * @return Current tick count in milliseconds
 */
static uint32_t lv_get_ticks(void)
{
    return app_lvglData.time_ticks;
}

/**
 * @brief LVGL display flush callback for SAM9X60 LCDC
 *
 * Called by LVGL when a rendering area is ready to be transferred to the display.
 * In direct render mode, switches the LCDC DMA address to the new framebuffer.
 * In partial render mode, uses blitBuffer to copy the rendered region.
 *
 * @param disp_drv Pointer to the LVGL display driver instance
 * @param area     Pointer to the rectangular area that was rendered (x1,y1 to x2,y2)
 * @param color_p  Pointer to the pixel data buffer in RGB565 format
 */
static void lv_disp_drv_flush_cb(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p)
{
#if APP_USE_DIRECT_RENDER_MODE
    LCDC_SetDMAAddressRegister(LCDC_LAYER_BASE, (uint32_t) color_p);
#else
    gfxPixelBuffer buf;

    buf.mode = GFX_COLOR_MODE_RGB_565;
    buf.size.width = area->x2 - area->x1 + 1;
    buf.size.height = area->y2 - area->y1 + 1;
    buf.pixel_count = buf.size.width * buf.size.height;
    buf.buffer_length = buf.pixel_count * sizeof(uint16_t);
    buf.pixels = (gfxBuffer) color_p;
    buf.orientation = GFX_ORIENT_0;
    buf.flags = 0;
    gfxDriverInterface.blitBuffer(area->x1, area->y1, &buf);
#endif

    lv_disp_flush_ready(disp_drv);
}

/**
 * @brief LVGL input device read callback
 *
 * Called by LVGL to poll the current state of the touch input device.
 * Reports the current touch coordinates and pressed/released state
 * based on the values updated by the touch event handlers.
 *
 * @param _          Unused pointer to the LVGL input device instance
 * @param indev_data Pointer to structure to fill with current input state
 */
static void lv_indev_drv_read_cb(lv_indev_t * _, lv_indev_data_t * indev_data)
{
    if (app_lvglData.touch_pressed) {
        indev_data->point.x = app_lvglData.touch_x;
        indev_data->point.y = app_lvglData.touch_y;
        indev_data->state = LV_INDEV_STATE_PRESSED;
    } else {
        indev_data->point.x = app_lvglData.touch_x;
        indev_data->point.y = app_lvglData.touch_y;
        indev_data->state = LV_INDEV_STATE_RELEASED;
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
    void APP_LVGL_Initialize ( void )

  Remarks:
    See prototype in app_lvgl.h.
 */

void APP_LVGL_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_lvglData.state = APP_LVGL_STATE_INIT;
    
    memset(&app_lvglData, 0, sizeof(app_lvglData));

    /* Initialize Input System Service */
    app_lvglData.input_listener.handleTouchDown = &touchDownHandler;
    app_lvglData.input_listener.handleTouchUp = &touchUpHandler;
    app_lvglData.input_listener.handleTouchMove = &touchMoveHandler;

    app_lvglData.touch_pressed = false;

    SYS_INP_AddListener(&app_lvglData.input_listener);
}


/******************************************************************************
  Function:
    void APP_LVGL_Tasks ( void )

  Remarks:
    See prototype in app_lvgl.h.
 */
void APP_LVGL_Tasks ( void )
{
    static uint32_t temp_ticks;
    
    /* Check the application's current state. */
    switch ( app_lvglData.state )
    {
        /* Application's initial state. */
        case APP_LVGL_STATE_INIT:
        {
            bool appInitialized = true;

            /* Initialize LVGL */
            lv_init();

            lv_tick_set_cb(lv_get_ticks);

            /* Set Active Layer */
            gfxIOCTLArg_Value ioctlArg;
            ioctlArg.value.v_uint = 0;
            gfxDriverInterface.ioctl(GFX_IOCTL_SET_ACTIVE_LAYER, &ioctlArg);  

            ioctlArg.value.v_pbuffer = 0;
            gfxDriverInterface.ioctl(GFX_IOCTL_GET_FRAMEBUFFER, &ioctlArg);            
            
            /* Get Display Parameters */
            gfxIOCTLArg_DisplaySize argDispSize;
            gfxDriverInterface.ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, &argDispSize);
                        
            /* Display */
            lv_display_t * display = lv_display_create(argDispSize.width, argDispSize.height);
#if APP_USE_DIRECT_RENDER_MODE            
            lv_display_set_buffers(display, ioctlArg.value.v_pbuffer->pixels, buff, sizeof(buff), LV_DISPLAY_RENDER_MODE_DIRECT);
#else
            lv_display_set_buffers(display, buff, NULL, sizeof(buff), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
            lv_display_set_flush_cb(display, lv_disp_drv_flush_cb);

            /* Input */
            lv_indev_t *indev = lv_indev_create();
            lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
            lv_indev_set_read_cb(indev, lv_indev_drv_read_cb);

            /* Periodic tick for LVGL */
            SYS_TIME_CallbackRegisterMS(lv_tick_inc_cb, 0, LV_TICK_INC_VAL_MS,
                    SYS_TIME_PERIODIC);
          
            /* Demo */
            #if LV_USE_DEMO_WIDGETS && !LV_USE_DEMO_BENCHMARK
            lv_demo_widgets();
            #elif LV_USE_DEMO_WIDGETS && LV_USE_DEMO_BENCHMARK
            lv_demo_widgets();
            lv_demo_benchmark();
            #elif LV_USE_DEMO_STRESS
            lv_demo_stress();
            #elif LV_USE_DEMO_MUSIC
            lv_demo_music();
            #elif LV_USE_DEMO_FLEX_LAYOUT
            lv_demo_flex_layout();
            #endif
            
            if (appInitialized)
            {

                app_lvglData.state = APP_LVGL_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_LVGL_STATE_SERVICE_TASKS:
        {
            if ((app_lvglData.time_ticks - temp_ticks) >= LV_TASK_INC_VAL_MS)
            {
                lv_timer_handler();

                temp_ticks = app_lvglData.time_ticks;
            }
          
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

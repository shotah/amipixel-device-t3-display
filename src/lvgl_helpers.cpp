// src/lvgl_helpers.cpp
#include "lvgl_helpers.h"

#define LVGL_TICK_PERIOD_MS 1

TFT_eSPI *tft; // Keep TFT_eSPI object declaration

// --- MOVE THESE GLOBAL DECLARATIONS OUTSIDE lvgl_driver_init() ---
static lv_draw_buf_t draw_buf;
static lv_color_t *buf;
static lv_display_driver_t disp_drv; // Corrected type name to lv_display_driver_t


/* Display flushing */
void my_disp_flush(lv_display_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) // Updated to lv_display_drv_t
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft->startWrite();
    tft->setAddrWindow(area->x1, area->y1, w, h);
    tft->pushColors((uint16_t *)&color_p->red, w * h, true);
    tft->endWrite();

    lv_display_flush_ready(disp); // Renamed to lv_display_flush_ready (though this one wasn't causing errors, for consistency)
}

/*Initialize дисплей and input devices*/
void lvgl_driver_init(void)
{
    // static lv_display_drv_t disp_drv;  <-- REMOVE THIS LOCAL DECLARATION - we use the global one now // Renamed to lv_display_drv_t
    lv_display_drv_init(&disp_drv);     /*Basic initialize*/ // Renamed to lv_display_drv_init

    /* дисплей resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;

    /* Used to copy the buffer's content to the display */
    disp_drv.flush_cb = my_disp_flush;

    buf = (lv_color_t *)heap_caps_malloc(screenWidth * 10 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    lv_draw_buf_init(&draw_buf, screenWidth, 10, LV_COLOR_FORMAT_RGB565, screenWidth * lv_color_format_get_size(LV_COLOR_FORMAT_RGB565), NULL, 0); // Corrected lv_draw_buf_init call

    disp_drv.draw_buf = &draw_buf;        /* Assign the buffer to the display */
    lv_display_drv_register(&disp_drv);      /* Finally register the driver */ // Renamed to lv_display_drv_register

    /*Initialize the touch interface*/
    // touch_driver_init(); // Touch driver initialization is commented out in this example
}

// Timer for LVGL tick
hw_timer_t *lvgl_timer = NULL;
void IRAM_ATTR lvgl_tick_handler(void)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void lvgl_tick_init(uint32_t tick_period_ms)
{
    lvgl_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(lvgl_timer, &lvgl_tick_handler, true);
    timerAlarmWrite(lvgl_timer, 80000 * tick_period_ms / 1000, true);
    timerAlarmEnable(lvgl_timer);
}